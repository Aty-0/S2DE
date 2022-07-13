Texture2D sprite_texture;
SamplerState sprite_sample_type;

struct PSINPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : UV;
    float3 worldPos		: POSITION; 
};

float4 main(PSINPUT input) : SV_TARGET
{
    return sprite_texture.Sample(sprite_sample_type, input.uv) * input.color;
}