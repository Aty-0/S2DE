Texture2D _texture;
SamplerState sample_type;

//TODO: Rename shader to geometry      

#define TYPE_DEFAULT_LIGHT 0 // It's class without lighting type or just invalid
#define TYPE_DIRECTIONAL_LIGHT 1
#define TYPE_POINT_LIGHT 2
#define TYPE_SPOT_LIGHT 3

#define MAX_LIGHTS 8 // TODO: Get number from engine

cbuffer CB_Main : register(b0)
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

struct PS_AmbientLight_Structure // Basic ambient light type 
{
	float4  color;
	float	strength;
};

struct PS_Light_Structure
{
    float4 position;
    float4 direction;
    float4 color;
    float3 attenuation;

    float spotAngle;
    float range;
    float pad;
    float strength;

    int light_type;
    int enabled;    
};

cbuffer CB_Light : register(b1)
{
    PS_Light_Structure lights[MAX_LIGHTS];
    PS_AmbientLight_Structure ambient_light;
};

struct LightData
{
    // TODO: Specular 

    float4 diffuse;
};

struct PSINPUT
{
    float4 position : SV_POSITION;
    float4 color : COLOR; // TODO: We need to remove color ? Because we have material emissive 
    float2 uv : UV;
    float3 normal : NORMAL;
    float3 worldPos		: POSITION; 
};

float4 BuildDiffuse(PS_Light_Structure light, float3 L, float3 N)
{
    float NdotL = max(0, dot(N, L));
    return light.color * NdotL;
}

LightData DoDirectionalLight(PS_Light_Structure light, float3 V, float4 P, float3 N)
{
    LightData result;

    float3 lightVec = -light.direction;
    // Add diffuse and specular term, provided the surface is in
    // the line of site of the light
    float diffuseFactor = dot(lightVec, N);

    // Flatten to avoid dynamic branching.
    [flatten]
    if (diffuseFactor > 0.0f)
    {
        result.diffuse = diffuseFactor * light.color;
    }

    return result;
}

LightData DoPointLight(PS_Light_Structure light, float3 V, float4 P, float3 N)
{
    LightData result = { { 0, 0, 0, 0 } };

    // The vector from the surface to the light.
    float3 lightVec = light.position - P;
    
    // The distance from surface to light.
    float d = length(lightVec);
    
    // Range test.
    if (d > light.range)
    {
  
        return result;
    }
       
    // Normalize the light vector.
    lightVec /= d;
    
    // Ambient term.
    //ambient = mat.Ambient * L.Ambient;
    
    // Add diffuse and specular term, provided the surface is in
    // the line of site of the light.

    float diffuseFactor = dot(lightVec, N);

    // Flatten to avoid dynamic branching.
    [flatten]
    if (diffuseFactor > 0.0f)
    {
        result.diffuse = light.range * diffuseFactor * light.color;
    }

    // Attenuate
    float att = 1.0f / dot(light.attenuation, float3(1.0f, d, d * d)) * light.strength;
    result.diffuse *= att;
    result.diffuse = saturate(result.diffuse);
    return result;
}

static PS_Light_Structure currentLight;
LightData ComputeLight(float4 Pos, float3 Normal)
{
    float3 V = normalize(float4(cameraPosition, 1) - Pos).xyz;
    LightData totalResult = { { 0, 0, 0, 0 } };

    [unroll(MAX_LIGHTS)]
    for (int i = 1; i < MAX_LIGHTS; ++i)
    {
        currentLight = lights[i];
        LightData result;

        if (!currentLight.enabled)
            continue;
    
        switch (currentLight.light_type)
        {
            case TYPE_SPOT_LIGHT:
                break;
            case TYPE_POINT_LIGHT:
                result = DoPointLight(currentLight, V, Pos, Normal);
                break;
            case TYPE_DIRECTIONAL_LIGHT:
                result = DoDirectionalLight(currentLight, V, Pos, Normal);
                break;
        }
             
        totalResult.diffuse += result.diffuse;        
    }

    totalResult.diffuse = saturate(totalResult.diffuse);    
    return totalResult;
}



float4 main(PSINPUT input) : SV_TARGET
{
    LightData lit = ComputeLight(float4(input.worldPos, 1), normalize(input.normal));
      
    float4 texColor = _texture.Sample(sample_type, input.uv);    
    float4 globalAmbient = ambient_light.color * ambient_light.strength;
    float4 finalColor = (texColor * (lit.diffuse + (globalAmbient / 2)));
    return finalColor;
}