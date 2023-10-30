struct VS_Input
{
    float3 WorldPosition : POSITION0;
    float3 LocalPosition : TEXCOORD0;
    float4 Color : COLOR;
    float Thickness: TEXCOORD1;
    float Fade: TEXCOORD2;
    nointerpolation int EntityID : TEXCOORD3;
};

cbuffer VS_Cbuffer : register(b0)
{
    matrix viewProjection;
}

struct VS_Output
{
    float4 WorldPosition : SV_POSITION;
    float3 LocalPosition : TEXCOORD0;
    float4 Color : COLOR;
    float Thickness: TEXCOORD1;
    float Fade: TEXCOORD2;
    nointerpolation int EntityID : TEXCOORD3;
};

VS_Output vs_main(VS_Input input)
{
    VS_Output output;
    output.LocalPosition = input.LocalPosition;
    output.Color = input.Color;
    output.Thickness = input.Thickness;
    output.Fade = input.Fade;
    
    output.EntityID = input.EntityID;
    
    output.WorldPosition = mul(float4(input.WorldPosition, 1.0f), viewProjection);

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
    
    float distance = 1.0 - length(input.LocalPosition);
    float circle = smoothstep(0.0, input.Fade, distance);
    circle *= smoothstep(input.Thickness + input.Fade, input.Thickness, distance);

    if (circle == 0.0)
        discard;

    // Set output color
    output.Color = input.Color;
    output.Color.a *= circle;

    output.EntityID = input.EntityID;
    return output;
}
