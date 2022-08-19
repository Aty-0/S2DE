cbuffer CB_Main  : register(b0)
{
    float Delta;
	float Time;
	float2 Resoultion;
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
    float3 cameraPosition;
}

struct VSINPUT
{
    float4 position : POSITION;
    float4 color : COLOR;
    float2 uv : UV;
    float3 normal : NORMAL;
};

struct PSINPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR;
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 worldPos		: POSITION; 
};


PSINPUT main(VSINPUT input)
{
    PSINPUT output;
    
    input.position.w = 1.0f;
    matrix worldViewProj = mul( mul( worldMatrix, viewMatrix ), projectionMatrix);
    output.position = mul(input.position, worldViewProj );
    output.worldPos = mul(input.position, worldMatrix).xyz;
    output.color = input.color;    
    output.uv = input.uv; 
    output.normal = normalize(mul(input.normal, worldMatrix));
    
    return output;
}
