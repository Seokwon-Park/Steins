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
    float2 uv : TEXCOORD0;
};

cbuffer VS_Cbuffer : register(b0)
{
    matrix viewProjection;
}

struct VS_Output
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VS_Output vs_main(VS_Input input)
{
    VS_Output output;
    output.position = mul(float4(input.pos, 1.0f), viewProjection);
    output.color = input.color;

    return output;
}

float4 ps_main(VS_Output input) : SV_TARGET
{
    return input.color;
}
