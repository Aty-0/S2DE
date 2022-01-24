//Global buffer 
cbuffer MainConstBuffer  : register(b0)
{
    float Delta;
	float Time;
	float2 Resoultion;
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
}

//Sprite buffer 
cbuffer SpriteConstBuffer  : register(b1)
{
    int   sprite_tile_frame_x;
    int   sprite_tile_frame_y;
    float2 sprite_tile_size;
    float2 sprite_texture_res;
}

struct VSINPUT
{
    float4 position : POSITION;
    float4 color : COLOR;
    float2 uv : UV;
};

struct PSINPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : UV;
    float3 worldPos		: POSITION; 
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
    
    matrix worldViewProj = mul( mul( worldMatrix, viewMatrix ), projectionMatrix);
    output.position = mul(input.position, worldViewProj );
    output.worldPos = mul(input.position, worldMatrix).xyz;
    output.color = input.color;
    output.uv = (sprite_tile_size.x == 0 && sprite_tile_size.y == 0) ? input.uv : GetAtlasUV(input.uv);
    
    return output;
}
