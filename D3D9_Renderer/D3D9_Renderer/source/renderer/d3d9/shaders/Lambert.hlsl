//Matrices
uniform extern float4x4 g_worldViewProjMatrix;
uniform extern float4x4 g_WorldMat;

//Directional Light
uniform extern float4 g_dirLightDir;
uniform extern float4 g_dirLightColor;

//Ambient Light
uniform extern float g_ambientLightIntensity;

struct VS_OUTPUT
{
    float4 position : POSITION0;
    float3 normal : NORMAL0;
};

VS_OUTPUT RenderVS(float3 pos : POSITION0,
    float3 norm : NORMAL0)
{
    VS_OUTPUT vsoutput = (VS_OUTPUT)0;
    vsoutput.position = mul(float4(pos, 1.0f), g_worldViewProjMatrix);
    vsoutput.normal = mul(float4(norm, 0.0f), g_WorldMat).xyz;
    return vsoutput;
}

struct PS_OUTPUT
{
    float4 RGBAcolor : COLOR0;
};

PS_OUTPUT RenderPS(in VS_OUTPUT IN)
{
    PS_OUTPUT psoutput;
    float luminence = saturate(dot(normalize(IN.normal), normalize(g_dirLightDir)));
    psoutput.RGBAcolor = (g_dirLightColor * luminence) + g_ambientLightIntensity;
    return psoutput;
}

technique LambertTech
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