#define TYPE_DEFAULT_LIGHT 0 // It's class without lighting type or just invalid
#define TYPE_DIRECTIONAL_LIGHT 1
#define TYPE_POINT_LIGHT 2

#define TYPE_SPOT_LIGHT 3
#define MAX_LIGHTS 64 // TODO: Get number from engine

// TODO: Rename shader to geometry      
TextureCube _cube : register(t1);

// TODO: We need to create material class
Texture2D _texture : register(t3);
SamplerState sample_type : register(s3);


cbuffer CB_Geom : register(b3)
{
    int texturesArraySize;
    int mat_index;
}

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
    float spot;

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

LightData DoDirectionalLight(PS_Light_Structure light, float4 P, float3 N)
{
    LightData result = { { 0, 0, 0, 0 } };
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

LightData DoSpotLight(PS_Light_Structure light, float4 P, float3 N)
{
    LightData result = { { 0, 0, 0, 0 } };
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

    // Add diffuse and specular term, provided the surface is in
    // the line of site of the light.
    float diffuseFactor = dot(lightVec, N);

    // Flatten to avoid dynamic branching.
    [flatten]
    if (diffuseFactor > 0.0f)
    {
        result.diffuse = light.range * diffuseFactor * light.color;
    }
    
    // Scale by spotlight factor and attenuate.
    float spot = pow(max(dot(-lightVec, radians(normalize(light.direction))), 0.0f), light.spot * 2);
    // Scale by spotlight factor and attenuate.
    float att = spot / dot(light.attenuation, float3(1.0f, d, d * d));
    result.diffuse *= att;
    result.diffuse = saturate(result.diffuse);

    return result;
}

LightData DoPointLight(PS_Light_Structure light, float4 P, float3 N)
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
    
    float refractiveIndex = 3.5f;
    float4 RD = refract(normalize(float4(cameraRotation,1)), 
               float4(N, 1), 1.0f / refractiveIndex);

    float4 R =  reflect( normalize(light.position), float4(N, 1.0f));

    // Flatten to avoid dynamic branching.
    [flatten]
    if (diffuseFactor > 0.0f)
    {
        result.diffuse = light.range * diffuseFactor * light.color;
    }

    // Attenuate
    float att = 1.0f / dot(light.attenuation, float3(1.0f, d, d * d)) * light.strength;
    result.diffuse *= att;
    result.diffuse = saturate(result.diffuse) + dot(-RD, result.diffuse);
    return result;
}

static PS_Light_Structure currentLight;
LightData ComputeLight(float4 Pos, float3 Normal)
{
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
                result = DoSpotLight(currentLight, Pos, Normal);
                break;
            case TYPE_POINT_LIGHT:
                result = DoPointLight(currentLight, Pos, Normal);
                break;
            case TYPE_DIRECTIONAL_LIGHT:
                result = DoDirectionalLight(currentLight, Pos, Normal);
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
    float ratio = 1.00 * min(length(lit.diffuse), 1) + (length(globalAmbient) * 0.1);
    //float4 texColor = _texture.Sample(sample_type, float3(input.uv, mat_index));
    float3 dir = refract(normalize(input.worldPos.xyz - cameraPosition), -normalize(input.normal), ratio);
    float4 cube = _cube.Sample(sample_type, dir);
    
    float4 finalColor = (saturate(texColor) + (saturate(cube) + float4(0,0,0.2f,1) * 0.5)) * ((lit.diffuse + saturate(globalAmbient)));
    return finalColor;
}