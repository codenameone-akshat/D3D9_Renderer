//Matrices
uniform extern float4x4 g_worldViewProjMatrix;
uniform extern float4x4 g_WorldMat;

//Directional Light
uniform extern float4 g_dirLightDir;
uniform extern float4 g_dirLightColor;

//Ambient Light
uniform extern float4 g_ambientLight;

//Speculatiry
uniform extern float g_specIntensity;
uniform extern float4 g_viewDirection;
uniform extern float4 g_specularLightColor;


struct VS_OUTPUT
{
    float4 position : POSITION0;
    float3 normal : NORMAL0;
    float3 worldPos : TEXCOORD0;
};

VS_OUTPUT RenderVS(float3 pos : POSITION0,
    float3 norm : NORMAL0)
{
    VS_OUTPUT vsoutput = (VS_OUTPUT)0;
    vsoutput.position = mul(float4(pos, 1.0f), g_worldViewProjMatrix);
    vsoutput.normal = mul(norm, (float3x3)g_WorldMat);
    vsoutput.worldPos = mul(pos, g_WorldMat); //get the light and vertex in the same matrix
    return vsoutput;
}

struct PS_OUTPUT
{
    float4 RGBAcolor : COLOR0;
};

PS_OUTPUT RenderPS(in VS_OUTPUT psInput)
{
    PS_OUTPUT psoutput = (PS_OUTPUT)0;

    float3 vWorldNormal = normalize(psInput.normal);
    float3 lightDir = normalize(g_dirLightDir.xyz);

    float4 diffuse = g_dirLightColor * saturate(dot(normalize(vWorldNormal), lightDir));

    float3 viewDir = normalize(g_viewDirection.xyz - psInput.worldPos); //get vector from vertex to light
    float3 halfVec = normalize(float3(viewDir + lightDir));
    
    float NoH = saturate(dot(vWorldNormal, halfVec));

    float4 specular = g_specularLightColor * pow(NoH, g_specIntensity);

    psoutput.RGBAcolor = saturate(diffuse + specular + g_ambientLight);

    return psoutput;
}

technique PhongSpecTech
{
    pass P0
    {
        VertexShader = compile vs_3_0 RenderVS();
        PixelShader = compile ps_3_0 RenderPS();

        ShadeMode = PHONG;
        FillMode = SOLID;
        CullMode = CCW;
    }
};