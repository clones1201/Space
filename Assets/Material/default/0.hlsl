#define Float4x4 float4x4
#define Float4 float4

static const float3 DirectionalLightDirection = normalize(float3(1.0f, -1.0f, -1.0f));
static const float3 DirectionalLightColor = float3(1.0f, 1.0f, 1.0f);	//should mul Intensity

#define Pi 3.1415926
#define DualPi 6.2831852

struct MatrixPack
{
	matrix<float,4,4> mProjection;
	Float4x4 mWorld;
};

tbuffer Bone{
	Float4x4 g_mTexBoneWorld[256];
};

Buffer<float4> g_Buffer;

cbuffer UpdateEveryFrame
{
	Float4x4 mView;
	MatrixPack pack;
};

Texture2D texture1;
Texture2D texture2;

SamplerState sampler1{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};
SamplerState sampler2
{
	Filter = MIN_MAG_MIP_LINEAR;
	AddressU = Wrap;
	AddressV = Wrap;
};

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
		output = mul(input, pack.mWorld);
	output = mul(output, mView);
	//output = mul(output, pack.pack.mProjection);
	return output;
}

void VS(VSInput input,out VSOutput output)
{
	output = (VSOutput)0;
	output.Position = PositionTransform(input.Position);
	output.PositionWS = mul(float4(input.Position.xyz, 1.0f), pack.mWorld);
	output.Normal = mul(float4(input.Normal.xyz, 0.0f), pack.mWorld);
	output.Tangent = mul(float4(input.Tangent.xyz, 0.0f), pack.mWorld);
	output.TexCoord = input.TexCoord;   
}

void PS(VSOutput vsoutput,out Float4 OutColor : SV_Target)
{	
	float4 color1 = texture1.Sample(sampler1, vsoutput.TexCoord.xy);
	float4 color2 = texture2.Sample(sampler2, vsoutput.TexCoord.xy);
	color1 = color1 + pack.mWorld[0];
	color2 = color2 + mView[0] + g_mTexBoneWorld[0][3] + g_Buffer.Load(2);
	OutColor = saturate(color1 - color2);
}