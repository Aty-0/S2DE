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

PSINPUT main(VSINPUT input)
{
    PSINPUT output;
    input.position.w = 1.0f;
    
    matrix modelView = viewMatrix;   
    matrix worldViewProj = mul(mul(worldMatrix, modelView), projectionMatrix);
    // Multiply worldViewProj matrix and input position
    output.position = mul(input.position, worldViewProj);
    output.worldPos = mul(input.position, worldMatrix).xyz;
    output.color = input.color;    
    output.uv = input.uv;

    return output;
}
