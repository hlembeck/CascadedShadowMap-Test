#include "BasicDirectionalLight.hlsl"


cbuffer CameraConstants : register(b1) {
	matrix viewProj;
	float4 direction;
	float4 position;
};

struct VSInput {
	float4 pos : POSITION;
	float4 n : NORMAL;
};

struct VSOutput {
	float4 pos : SV_POSITION;
	float4 wPos : WPOS;
	float4 n : NORMAL;
};

VSOutput VS(VSInput input) {
	VSOutput output = (VSOutput)0;
	output.wPos = input.pos;
	output.pos = mul(input.pos,viewProj);
	output.n = input.n;
	return output;
}

float4 PS(VSOutput input) : SV_TARGET{
	input.n = normalize(input.n);
	return GetDirectionalLightFactor(input.wPos,-LIGHTDIR,input.n,direction,position);
}