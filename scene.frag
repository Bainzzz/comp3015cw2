#version 460

in vec3 FragPos;
in vec3 Normal;
in vec2 TexCoord;
in vec4 FragPosLightSpace;

layout (location = 0) out vec4 FragColor;

layout(binding = 0) uniform sampler2D BallTex;
layout(binding = 1) uniform sampler2DShadow ShadowMap;

uniform vec3 LightPosition; 

uniform struct MaterialInfo {
    vec3  Ka;
    vec3  Kd;
    vec3  Ks;
    float Shininess;
} Material;

uniform struct LightInfo {
    vec3  La;
    vec3  Ld;
    vec3  Ls;
    vec3  Direction; 
    float Cutoff;     
    float Exponent;  
} Light;

uniform bool HasTexture;

float shadowFactor(vec4 fragPosLS)
{
    vec3 proj = fragPosLS.xyz / fragPosLS.w;
    proj = proj * 0.5 + 0.5;

    if (proj.z > 1.0) return 1.0;

    float shadow = 0.0;
    vec2 texelSize = 1.0 / textureSize(ShadowMap, 0);

    for (int x = -1; x <= 1; x++) {
        for (int y = -1; y <= 1; y++) {
            shadow += texture(ShadowMap,
                              vec3(proj.xy + vec2(x, y) * texelSize, proj.z));
        }
    }
    return shadow / 9.0;
}

vec3 blinnPhong(vec3 pos, vec3 norm, vec3 diffCol, float shadow)
{
    vec3 ambient  = Light.La * Material.Ka;
    vec3 lightDir = normalize(LightPosition - pos);

    float cosAngle   = dot(-lightDir, normalize(Light.Direction));
    float innerCutoff = Light.Cutoff;
    float outerCutoff = Light.Cutoff - 0.05;
    float spotEffect  = smoothstep(outerCutoff, innerCutoff, cosAngle);

    float diff   = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = Light.Ld * diffCol * diff;

    vec3  viewDir = normalize(-pos);
    vec3  halfway = normalize(lightDir + viewDir);
    float spec    = pow(max(dot(norm, halfway), 0.0), Material.Shininess);
    vec3 specular = Light.Ls * Material.Ks * spec;

    float dist  = length(LightPosition - pos);
    float atten = 1.0 / (1.0 + 0.09 * dist + 0.032 * dist * dist);

    return ambient + shadow * spotEffect * atten * (diffuse + specular);
}

void main()
{
    vec3  diffCol = HasTexture ? texture(BallTex, TexCoord).rgb : Material.Kd;
    float shadow  = shadowFactor(FragPosLightSpace);
    vec3  color   = blinnPhong(FragPos, normalize(Normal), diffCol, shadow);
    FragColor     = vec4(color, 1.0);
}