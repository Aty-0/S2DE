//Global buffer 
cbuffer CB_Main  : register(b0)
{
    float Delta;
	float Time;
	float2 Resoultion;
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
}

//Sprite buffer 
cbuffer CB_Sprite  : register(b1)
{
    float2  tileFrame;
    float2  tileSize;
    float2  textureRes;
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
    float2 size = float2(1.0f  / (length(textureRes) / tileSize.x), 1.0f / (length(textureRes) / tileSize.y));
    float2 offset = float2(size.x * tileFrame.x, size.y * tileFrame.y);   
    float2 tileUV = uv  * size + offset;
    return tileUV;
}

PSINPUT main(VSINPUT input)
{
    PSINPUT output;
    
    input.position.w = 1.0f;
    matrix worldViewProj = mul( mul( worldMatrix, viewMatrix ), projectionMatrix);
    output.position = mul(input.position, worldViewProj );
    output.worldPos = mul(input.position, worldMatrix).xyz;
    output.color = input.color;    
    output.uv = (tileSize.x == 0 && tileSize.y == 0) ? input.uv : GetAtlasUV(input.uv);
    
    return output;
}
