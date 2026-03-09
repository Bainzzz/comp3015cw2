#version 460

in vec3 FragPos;
in vec3 Normal;

layout (location = 0) out vec4 FragColor;

uniform struct LightInfo {
    vec4 Position;
    vec3 La;
    vec3 Ld;
    vec3 Ls;
    vec3 Direction;
    float Cutoff;
    float Exponent;
} Light;

uniform struct MaterialInfo {
    vec3 Ka;
    vec3 Kd;
    vec3 Ks;
    float Shininess;
} Material;

uniform struct FogInfo {
    float MaxDist;
    float MinDist;
    vec3 Color;
} Fog;

vec3 blinnPhongSpot(vec3 pos, vec3 norm)
{
    vec3 ambient = Light.La * Material.Ka;

    vec3 lightDir = normalize(vec3(Light.Position) - pos);
    
    // Check if fragment is inside spotlight cone
    float cosAngle = dot(-lightDir, normalize(Light.Direction));
    float spotEffect = 0.0;
    
    if(cosAngle > Light.Cutoff)
    {
        spotEffect = pow(cosAngle, Light.Exponent);
        
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = Light.Ld * Material.Kd * diff;

        vec3 viewDir = normalize(-pos);
        vec3 halfwayDir = normalize(lightDir + viewDir);
        float spec = pow(max(dot(norm, halfwayDir), 0.0), Material.Shininess);
        vec3 specular = Light.Ls * Material.Ks * spec;

        return ambient + spotEffect * (diffuse + specular);
    }
    
    return ambient;
}

void main()
{
    // Calculate fog factor based on distance from camera
    float dist = abs(FragPos.z);
    float fogFactor = clamp((Fog.MaxDist - dist) / (Fog.MaxDist - Fog.MinDist), 0.0, 1.0);

    vec3 shadeColor = blinnPhongSpot(FragPos, normalize(Normal));
    
    // Mix scene color with fog color
    vec3 color = mix(Fog.Color, shadeColor, fogFactor);
    FragColor = vec4(color, 1.0);  //fog doesnt come out how im wanting try again later 
}