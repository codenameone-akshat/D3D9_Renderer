uniform extern float4x4 g_worldViewProjMatrix;
uniform extern float g_randVal;

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
	vsoutput.position.y = sin(vsoutput.position.y) * 3.5f;
	return vsoutput;
}

struct PS_OUTPUT
{
	float4 RGBAcolor : COLOR0;
};

PS_OUTPUT RenderPS()
{
	PS_OUTPUT psoutput;
	psoutput.RGBAcolor = float4(0.8f, g_randVal, 0.0f, 0.8f);
	return psoutput;
}

technique TestTech
{
	pass P0
	{
		VertexShader = compile vs_3_0 RenderVS();
		PixelShader = compile ps_3_0 RenderPS();

		ShadeMode = PHONG;
		FillMode = WIREFRAME;
		CullMode = NONE;
	}
};