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
    float texIdx : TEXCOORD1;
    float tileFactor: TEXCOORD2;
    int entityID : TEXCOORD3;
};

cbuffer VS_Cbuffer : register(b0)
{
    matrix viewProjection;
}

struct VS_Output
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : TEXCOORD0;
    float texIdx : TEXCOORD1;
    float tileFactor : TEXCOORD2;
    int entityID : TEXCOORD3;
};

VS_Output vs_main(VS_Input input)
{
    VS_Output output;
    output.position = mul(float4(input.pos, 1.0f), viewProjection);
    output.color = input.color;
    output.uv = input.uv;
    output.texIdx = input.texIdx;
    output.tileFactor = input.tileFactor;
    output.entityID = input.entityID;

    return output;
}

//TODO: expand to 32size
Texture2D g_texture[2] : register(t0);
//Texture2D g_texture1 : register(t1);

SamplerState g_sampler : register(s0);

struct PS_Output
{
    float4 color : SV_Target0;
    int color2 : SV_Target1;
};

PS_Output ps_main(VS_Output input) 
{    
    float4 color;

    PS_Output output;
    // TODO:
    // switch(input.texIdx)
    // {
    // case 0:
    // }
    if(input.texIdx == 0)
        color = g_texture[0].Sample(g_sampler, input.uv * input.tileFactor)*input.color;
    else if(input.texIdx == 1)
        color = g_texture[1].Sample(g_sampler, input.uv * input.tileFactor)*input.color;
        
    output.color = color;
    output.color2 = input.entityID;
    return output;
}
