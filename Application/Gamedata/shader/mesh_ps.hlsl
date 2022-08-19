Texture2D _texture;
SamplerState sample_type;

struct ambient_ligth_t
{
	float3  color;
	float	strength;
};

cbuffer CB_Light : register(b1)
{
    ambient_ligth_t ambient_light;
};

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
    float3 lightDirection = normalize(input.position.xyz);

    float  _dot = max(0.0, dot(lightDirection, input.normal.xyz));
    float4 ambient = float4(ambient_light.color * ambient_light.strength, 1.0f);
    float4 diffuse = _texture.Sample(sample_type, input.uv) * input.color;
    float4 finalColor = diffuse * ambient *  _dot;  
    return finalColor; //float4(input.normal, 1); 
}