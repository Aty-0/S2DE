cbuffer MainConstBuffer  : register(b0)
{
    float Delta;
	float Time;
	float2 Resoultion;
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
}

struct VSINPUT
{
    float3 position : POSITION;
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

float random( float2 p )
{
    float2 K1 = float2(
        23.14069263277926, // e^pi (Gelfond's constant)
         2.665144142690225 // 2^sqrt(2) (Gelfondâ€“Schneider constant)
    );
    return frac( cos( dot(p,K1) ) * 12345.6789 );
}

PSINPUT main(VSINPUT input)
{
    PSINPUT output;
    matrix worldViewProj = mul( mul( worldMatrix, viewMatrix ), projectionMatrix);
    output.position = mul(float4( input.position, 1.0f ), worldViewProj );
    output.worldPos = mul( float4( input.position, 1.0f ), worldMatrix).xyz;
    output.color = input.color;
    
    return output;
}
