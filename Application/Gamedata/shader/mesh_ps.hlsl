Texture2D _texture;
SamplerState sample_type;


cbuffer MainConstBuffer  : register(b0)
{
    float Delta;
	float Time;
	float2 Resoultion;
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
}


struct PSINPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 worldPos		: POSITION; 
};

float4 main(PSINPUT input) : SV_TARGET
{
    input.normal = normalize(input.normal);
    float4 diffuse = _texture.Sample(sample_type, input.uv) * input.color;
    return  diffuse; //float4(input.normal,1.0f);
}