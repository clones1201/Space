#define Float4x4 float4x4
#define Float4 float4

static const float3 DirectionalLightDirection = normalize(float3(1.0f, -1.0f, -1.0f));
static const float3 DirectionalLightColor = float3(1.0f, 1.0f, 1.0f);	//should mul Intensity

#define Pi 3.1415926
#define DualPi 6.2831852

cbuffer UpdateEveryFrame
{
	Float4x4 mView;
	Float4x4 mWorld;
	Float4x4 mProjection; 
};

Texture2D texture1;
Texture2D texture2;

SamplerState sampler1;

struct VSInput
{
	float4 Position : POSITION0;
	float4 Normal   : NORMAL0;
	float4 Tangent  : TANGENT0;
	float4 TexCoord : TEXCOORD0;
};

struct VSOutput
{
	float4 Position  : SV_POSITION;
	float4 PositionWS: POSITION0;
	float4 Normal    : NORMAL0;
	float4 Tangent   : TANGENT0;
	float4 TexCoord  : TEXCOORD0;
};

float4 PositionTransform(float4 input){
	float4 output = (float4)0;
		output = mul(input, mWorld);
	output = mul(output, mView);
	output = mul(output, mProjection);
	return output;
}

void VS(VSInput input,out VSOutput output)
{
	output = (VSOutput)0;
	output.Position = PositionTransform(input.Position);
	output.PositionWS = mul(float4(input.Position.xyz, 1.0f), mWorld);
	output.Normal = mul(float4(input.Normal.xyz, 0.0f), mWorld);
	output.Tangent = mul(float4(input.Tangent.xyz, 0.0f), mWorld);
	output.TexCoord = input.TexCoord;   
}

void PS(VSOutput vsoutput,out Float4 OutColor : SV_Target, uniform bool TextureSwitch)
{	
	float4 color1 = texture1.Sample(sampler1, vsoutput.TexCoord.xy);
	float4 color2 = texture2.Sample(sampler1, vsoutput.TexCoord.xy);
	OutColor = saturate(color1 - color2);
}