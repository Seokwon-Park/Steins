struct VS_Input
{
    float3 pos : POSITION;
    float4 color : COLOR;
};

struct VS_Output
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

VS_Output vs_main(VS_Input input)
{
    VS_Output output;
    output.position = float4(input.pos, 1.0f);
    output.color = input.color;

    return output;
}

float4 ps_main(VS_Output input) : SV_TARGET
{
    return input.color;
}