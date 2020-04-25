#define Float4x4 float4x4
#define Float4x3 float4x3
#define Float3x4 float3x4
#define Float4 float4

static const float3 DirectionalLightDirection = normalize(float3(1.0f, -1.0f, -1.0f));
static const float3 DirectionalLightColor = float3(1.0f, 1.0f, 1.0f);	//should mul Intensity

#define Pi 3.1415926
#define DualPi 6.2831852

tbuffer Bone{
	Float4x4 g_mTexBoneWorld[256];
};

Buffer<float4> g_Buffer;

cbuffer PerFrameData
{
	Float4x4 View;
	Float4x4 Projection;
};

cbuffer PerObjectData
{
	Float4x4 World;
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

struct MaterialVertexInputParameters
{
	// ModelView - View
	float4x4 MV;
	float4x4  Projection;
};

MaterialVertexInputParameters GetVertexInputParameter(VSInput input)
{
    MaterialVertexInputParameters ret = (MaterialVertexInputParameters) 0;
    ret.MV = mul( World, View);
    ret.Projection = Projection;
	return ret;
}

struct VSOutput
{
	float4 Position  : SV_POSITION;
	float4 PositionWS: POSITION1;
	float4 NormalWS    : NORMAL0;
	float4 Tangent   : TANGENT0;
	float4 TexCoord  : TEXCOORD0;
};

float4 PositionTransform(float4 input, MaterialVertexInputParameters parameters)
{
	float4 output = (float4)0;
	output = mul(input, parameters.MV);
	output = mul(output, parameters.Projection);
	return output;
}

void MainVS(in VSInput input,out VSOutput output)
{
    MaterialVertexInputParameters parameters = GetVertexInputParameter(input);
	output = (VSOutput)0;
    output.Position = PositionTransform(float4(input.Position.xyz, 1.0f), parameters);
	output.PositionWS = mul(float4(input.Position.xyz, 1.0f), parameters.MV);
	output.NormalWS = mul(float4(input.Normal.xyz, 0.0f), parameters.MV);
	output.Tangent = mul(float4(input.Tangent.xyz, 0.0f), parameters.MV);
	output.TexCoord = input.TexCoord;   
}

void MainPS(VSOutput vsoutput,out Float4 OutColor : SV_Target)
{	
	//float4 color1 = texture1.Sample(sampler1, vsoutput.TexCoord.xy);
	//float4 color2 = texture2.Sample(sampler2, vsoutput.TexCoord.xy);
   // color1 = color1 + vsoutput.Position;
    //color2 = color2 + vsoutput.PositionWS + g_mTexBoneWorld[0][3] + g_Buffer.Load(2);
    OutColor = float4(vsoutput.NormalWS.xyz, 1.0);
}