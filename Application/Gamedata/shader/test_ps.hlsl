Texture2D test_texture;
SamplerState test_sampleType;

struct PSINPUT
{
    float4 position : POSITION;
    float4 color : COLOR;
    float2 uv : UV0;
};

float hashOld12(float2 p)
{
	return frac(sin(dot(p, float2(12.9898, 78.233))) * 43758.5453);
}

float4 main(PSINPUT input) : SV_TARGET
{
    return test_texture.Sample(test_sampleType, input.uv);
}