cbuffer CB_Main  : register(b0)
{
    float Delta;
	float Time;
	float2 Resoultion;
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    float3 cameraPosition;
    float3 cameraRotation;
}

cbuffer CB_Sprite  : register(b2)
{
    float2  tileFrame;
    float2  tileSize;
    float2  textureRes;
    bool    billboard;
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
    
    matrix modelView = viewMatrix;
    
    if (billboard == true)
    {   
        // FIX ME: Rotation bug

        // Multiplication of the view matrix and the model matrix 
        //modelView[0][0] = 1.0;
        //modelView[0][1] = 0.0;
        //modelView[0][2] = 0.0;
        //modelView[1][0] = 0.0;
        //modelView[1][1] = 1.0;
        //modelView[1][2] = 0.0;
        //modelView[2][0] = 0.0;
        //modelView[2][1] = 0.0;
        //modelView[2][2] = 1.0;
    }
    
    matrix worldViewProj = mul(mul(worldMatrix, modelView), projectionMatrix);
    // Multiply worldViewProj matrix and input position
    output.position = mul(input.position, worldViewProj);
    output.worldPos = mul(input.position, worldMatrix).xyz;
    output.color = input.color;    

    // If atlas size is zero in both cordinates we are get input uv
    // If we are have the atlas size we get atlas uv
    output.uv = (tileSize.x == 0 && tileSize.y == 0) ? input.uv : GetAtlasUV(input.uv);

    return output;
}
