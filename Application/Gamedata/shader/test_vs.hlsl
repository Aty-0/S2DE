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


PSINPUT main(VSINPUT input)
{
    PSINPUT output;

    output.position = mul(float4(input.position, 1.0f), worldMatrix);
    output.position = mul(output.position, viewMatrix);
    output.position = mul(output.position, projectionMatrix);
    output.uv = input.uv;
    //output.uv += Time * 0.01f;
    
    float t = clamp(sin(Time), 0.5, 1);
    output.color = input.color; //float4(t + cos(Time * 3.14 / 360), t + sin(Time * 3.14 / 360), t, 255);
    
    return output;
}
