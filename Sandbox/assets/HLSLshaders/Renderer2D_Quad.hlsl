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
    nointerpolation float texIdx : TEXCOORD1;
    float tileFactor: TEXCOORD2;
    nointerpolation int entityID : TEXCOORD3;
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
    nointerpolation float texIdx : TEXCOORD1;
    float tileFactor : TEXCOORD2;
    nointerpolation int entityID : TEXCOORD3;
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
Texture2D g_texture[32] : register(t0);
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
    switch (input.texIdx)
    {
        case 0:color = g_texture[0].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 1:color = g_texture[1].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 2:color = g_texture[2].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 3:color = g_texture[3].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 4:color = g_texture[4].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 5:color = g_texture[5].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 6:color = g_texture[6].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 7:color = g_texture[7].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 8:color = g_texture[8].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 9:color = g_texture[9].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 10:color = g_texture[10].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 11:color = g_texture[11].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 12:color = g_texture[12].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 13:color = g_texture[13].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 14:color = g_texture[14].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 15:color = g_texture[15].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 16:color = g_texture[16].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 17:color = g_texture[17].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 18:color = g_texture[18].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 19:color = g_texture[19].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 20:color = g_texture[20].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 21:color = g_texture[21].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 22:color = g_texture[22].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 23:color = g_texture[23].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 24:color = g_texture[24].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 25:color = g_texture[25].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 26:color = g_texture[26].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 27:color = g_texture[27].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 28:color = g_texture[28].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 29:color = g_texture[29].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 30:color = g_texture[30].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
        case 31:color = g_texture[31].Sample(g_sampler, input.uv * input.tileFactor) * input.color; break;
    }
    //if(input.texIdx == 0)
    //    color = g_texture[0].Sample(g_sampler, input.uv * input.tileFactor)*input.color;
    //else if(input.texIdx == 1)
    //    color = g_texture[1].Sample(g_sampler, input.uv * input.tileFactor)*input.color;
    
        
    output.color = color;
    output.color2 = input.entityID;
    return output;
}
