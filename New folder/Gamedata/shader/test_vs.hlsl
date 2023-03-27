cbuffer CB_Main : register(b0)
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


PSINPUT main(VSINPUT input)
{
    PSINPUT output;
    matrix worldViewProj = mul( mul( worldMatrix, viewMatrix ), projectionMatrix);
    output.position = mul(float4( input.position, 1.0f ), worldViewProj );
    output.worldPos = mul( float4( input.position, 1.0f ), worldMatrix).xyz;
    output.uv = input.uv;
    //output.uv += Time * 0.01f;
    //output.position.w = 1;
    float t = clamp(sin(Time), 0.5, 1);
    output.color = float4(t + cos(Time * 3.14 / 360), t + sin(Time * 3.14 / 360), t, 255);
    
    return output;
}
