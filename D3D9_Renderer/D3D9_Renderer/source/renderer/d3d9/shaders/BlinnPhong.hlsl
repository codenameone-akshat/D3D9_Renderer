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
uniform extern texture g_SpecularTex;
uniform extern texture g_OpacityTex;

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
    MinFilter = Anisotropic;
    MagFilter = Linear;
    MipFilter = Linear;
	MaxAnisotropy = 4;
	AddressU = Wrap;
    AddressV = Wrap;
};

sampler SpecularSampler = sampler_state
{
	Texture = <g_SpecularTex>;
	MinFilter = Anisotropic;
	MagFilter = Linear;
	MipFilter = Linear;
	MaxAnisotropy = 4;
	AddressU = Wrap;
	AddressV = Wrap;
};

sampler OpacitySampler = sampler_state
{
	Texture = <g_OpacityTex>;
	MinFilter = Anisotropic;
	MagFilter = Linear;
	MipFilter = Linear;
	MaxAnisotropy = 4;
	AddressU = Clamp;
	AddressV = Clamp;
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
    vsoutput.normal = normalize(mul(norm, (float3x3)g_WorldMat));
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

    float3 bumpTex = 2.0f * tex2D(NormalSampler, psInput.uv) - 1.0f; //from 0 to 1 to -1 to 1
	float3 bumpedNormal = normalize(((bumpTex.x * psInput.tangent) + (bumpTex.y * psInput.biTangent) + (bumpTex.z * psInput.normal)));
	
    float3 lightDir = normalize(g_dirLightDir.xyz);
	
	float4 texColorDiff = tex2D(DiffuseSampler, psInput.uv);

    float4 diffuse = texColorDiff * g_dirLightColor * saturate(dot(bumpedNormal, lightDir));
	
    float3 viewDir = normalize(g_viewDirection.xyz - psInput.worldPos); //get vector from vertex to light
    float3 halfVec = normalize(float3(viewDir + lightDir));
    
    float NoH = saturate(dot(bumpedNormal, halfVec));

    float4 specular = g_specularLightColor * pow(NoH, g_specIntensity);
	float4 specMapIntensity = tex2D(SpecularSampler, psInput.uv);
	specular = specular * specMapIntensity;
	
	float4 opacity = tex2D(OpacitySampler, psInput.uv);

    psoutput.RGBAcolor = saturate(diffuse + specular + g_ambientLight);
	psoutput.RGBAcolor.a = opacity.a;
    return psoutput;
}

technique BlinnPhongTech
{
    pass P0
    {
		AlphaBlendEnable = TRUE;
		DestBlend = INVSRCALPHA;
		SrcBlend = SRCALPHA;
		ShadeMode = PHONG;
		FillMode = SOLID;
		CullMode = CCW;

        VertexShader = compile vs_3_0 RenderVS();
        PixelShader = compile ps_3_0 RenderPS();

    }
};