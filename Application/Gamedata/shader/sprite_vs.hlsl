cbuffer SpriteConstBuffer  : register(b0)
{
    float Delta;
	float Time;
	float2 Resoultion;
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
}

cbuffer SpriteConstBuffer  : register(b1)
{
    int   sprite_tile_frame_x;
    int   sprite_tile_frame_y;
    float2 sprite_tile_size;
    float2 sprite_texture_res;
}

struct VSINPUT
{
    float3 position : POSITION;
    float4 color : COLOR;
    float2 uv : UV0;
};

struct PSINPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : UV0;
};

float2 GetAtlasUV(float2 uv)
{
    float2 size = float2(1.0f  / (length(sprite_texture_res) / sprite_tile_size.x), 
    1.0f / (length(sprite_texture_res) / sprite_tile_size.y));

    float2 offset = float2(size.x * sprite_tile_frame_x, size.y * sprite_tile_frame_y);   
    float2 tileuv = uv  * size + offset;
    return tileuv;
}

PSINPUT main(VSINPUT input)
{
    PSINPUT output;

    output.position = mul(float4(input.position, 1.0f), worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.color = input.color;

    if(sprite_tile_size.x == 0 && sprite_tile_size.y == 0)
        output.uv = input.uv;
    else
        output.uv = GetAtlasUV(input.uv);

    return output;
}
