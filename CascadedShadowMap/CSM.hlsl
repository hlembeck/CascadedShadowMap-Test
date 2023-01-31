static const uint MAXFRUSTA = 10;

cbuffer Projections : register(b0) {
	matrix matrices[MAXFRUSTA];
};

struct GSOutput {
	float4 pos : SV_POSITION;
	uint slice : SV_RenderTargetArrayIndex;
};

float4 VS(float4 pos : POSITION) : SV_POSITION{
	return pos;
}

[maxvertexcount(30)]
void GS(triangle float4 input[3] : SV_POSITION, inout TriangleStream<GSOutput> OutStream)
{
	GSOutput output = (GSOutput)0;

	for (uint i = 0; i < MAXFRUSTA; i++) {
		output.slice = i;
		output.pos = mul(input[0], matrices[i]);
		OutStream.Append(output);

		output.pos = mul(input[1], matrices[i]);
		OutStream.Append(output);

		output.pos = mul(input[2], matrices[i]);
		OutStream.Append(output);

		OutStream.RestartStrip();
	}
}