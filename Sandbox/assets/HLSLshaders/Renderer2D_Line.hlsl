//struct v2f {
//    float4 pos : sv_position;
//    float4 myvectorvalue : texcoord0;
//    float myfloatvalue : texcoord1;
//    float3 mypositionvalue : texcoord2;
//}

struct VS_Input
{
	float3 pos : POSITION;
	float4 color : COLOR;
	nointerpolation int entityID : TEXCOORD0;
};

cbuffer VS_Cbuffer : register(b0)
{
	matrix viewProjection;
}

struct VS_Output
{
	float4 position : SV_POSITION;
	float4 color : COLOR;
	nointerpolation int entityID : TEXCOORD0;
};

VS_Output vs_main(VS_Input input)
{
	VS_Output output;
	output.position = mul(float4(input.pos, 1.0f), viewProjection);
	output.color = input.color;
	output.entityID = input.entityID;

	return output;
}

SamplerState g_sampler : register(s0);

struct PS_Output
{
	float4 Color : SV_Target0;
	int EntityID : SV_Target1;
};

PS_Output ps_main(VS_Output input)
{
	float4 color;

	PS_Output output;
        
	output.Color = color;
	output.EntityID = input.entityID;
	return output;
}
