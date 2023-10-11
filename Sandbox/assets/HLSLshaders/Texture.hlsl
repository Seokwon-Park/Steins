#type vertex
#version 330 core

struct VS_Input
{
    float3 pos : POSITION;
    float2 uv : TEXCOORD0;
};

struct VS_Output
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
};

layout(location = 0) in vec3 a_Position;
layout(location = 1) in vec2 a_TexCoord;

uniform mat4 u_ViewProjection;
uniform mat4 u_Transform;

out vec2 v_TexCoord;		

void main()
{
	v_TexCoord = a_TexCoord;
	gl_Position = u_ViewProjection * u_Transform * vec4(a_Position, 1.0);
}

#type fragment
#version 330 core

layout(location = 0) out vec4 color;

in vec2 v_TexCoord;

uniform vec4 u_Color;
uniform float u_TilingFactor;
uniform sampler2D u_Texture;

void main()
{
	color = texture(u_Texture, v_TexCoord * u_TilingFactor) * u_Color;
}
float4 main( float4 pos : POSITION ) : SV_POSITION
{
	return pos;
}