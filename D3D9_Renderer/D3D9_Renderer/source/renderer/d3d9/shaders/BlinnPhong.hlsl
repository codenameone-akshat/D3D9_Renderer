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

//Textures
uniform extern texture g_DiffuseTex;
uniform extern texture g_NormalTex;

sampler DiffuseSampler = sampler_state
{
    Texture = <g_DiffuseTex>;
    MinFilter = Anisotropic;
    MagFilter = Linear;
	MipFilter = Linear;
    MaxAnisotropy = 4;
    AddressU = Wrap;
    AddressV = Wrap;
};

sampler NormalSampler = sampler_state
{
    Texture = <g_NormalTex>;
    MinFilter = Linear;
    MagFilter = Linear;
    MipFilter = Linear;
    AddressU = Wrap;
    AddressV = Wrap;
};

struct VS_OUTPUT
{
    float4 position : POSITION0;
    float3 normal : NORMAL0;
    float3 worldPos : TEXCOORD0;
    float2 uv : TEXCOORD1;
    float3 tangent: TEXCOORD2;
    float3 biTangent: TEXCOORD3;
};

VS_OUTPUT RenderVS(float3 pos : POSITION0,
    float3 norm : NORMAL0,
	float2 uv : TEXCOORD0,
    float3 tangent : TANGENT0,
    float3 biTangent : BINORMAL0)
{
    VS_OUTPUT vsoutput = (VS_OUTPUT)0;
    vsoutput.position = mul(float4(pos, 1.0f), g_worldViewProjMatrix);
    vsoutput.normal = mul(norm, (float3x3)g_WorldMat);
    vsoutput.worldPos = mul(pos, g_WorldMat); //get the light and vertex in the same matrix
	vsoutput.uv = uv;
    vsoutput.tangent = normalize(mul(tangent, (float3x3)g_WorldMat));
    vsoutput.biTangent = normalize(mul(biTangent, (float3x3)g_WorldMat));
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
    float3 bumpTex = 2.0f * tex2D(NormalSampler, psInput.uv) - 1.0f; //from 0 to 1 to -1 to 1
    float3 bumpedNormal = normalize(vWorldNormal + ((bumpTex.x * psInput.tangent) + (bumpTex.y * psInput.biTangent)));
    
    float3 lightDir = normalize(g_dirLightDir.xyz);
	
	float4 texColorDiff = tex2D(DiffuseSampler, psInput.uv);

    float4 diffuse = texColorDiff * g_dirLightColor * saturate(dot(bumpedNormal, lightDir));
	
    float3 viewDir = normalize(g_viewDirection.xyz - psInput.worldPos); //get vector from vertex to light
    float3 halfVec = normalize(float3(viewDir + lightDir));
    
    float NoH = saturate(dot(bumpedNormal, halfVec));

    float4 specular = g_specularLightColor * pow(NoH, g_specIntensity);

    psoutput.RGBAcolor = saturate(diffuse + specular + g_ambientLight);

    return psoutput;
}

technique BlinnPhongTech
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