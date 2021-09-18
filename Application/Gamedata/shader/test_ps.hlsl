struct PSINPUT
{
    float4 position : POSITION;
    float4 color : COLOR;
    float2 uv : UV;
};

float hashOld12(float2 p)
{
	return frac(sin(dot(p, float2(12.9898, 78.233))) * 43758.5453);
}

////////////////////////////////////////////////////////////////////////////////
// Pixel Shader
////////////////////////////////////////////////////////////////////////////////
float4 main(PSINPUT input) : SV_TARGET
{
    return input.color;
}