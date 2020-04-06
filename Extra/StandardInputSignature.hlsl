// Shader with Standard Input Signature

struct VSInput
{
	float4 Position : POSITION0;
	float4 Normal   : NORMAL0;
	float4 Tangent  : TANGENT0;
	float4 TexCoord : TEXCOORD0;
};

float4 DummyVertexShader(in VSInput input ) : SV_POSITION
{
	return input.Position;
}