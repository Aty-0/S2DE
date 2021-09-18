cbuffer MainShaderBuffer
{
    float Delta;
	float Time;
	float2 Resoultion;
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

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
};


PSINPUT main(VSINPUT input)
{
    PSINPUT output;
    input.position.w = 1.0f;
    output.position = mul(input.position, worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.uv = input.uv;

    
    float t = clamp(sin(Time), 0.5, 1);
    output.color =  float4(t, 0, 0, 255);
    
    return output;
}
