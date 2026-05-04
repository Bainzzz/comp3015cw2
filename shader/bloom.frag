#version 460

in vec2 UV;
layout (location = 0) out vec4 FragColor;

uniform sampler2D HdrBuffer;
uniform bool      BloomEnabled;

const float exposure    = 1.4;  // overall brightness
const float bloomThresh = 0.95;  // brightness threshold
const float bloomStr    = 0.5;  // how strong the bloom glow is
const int   blurRadius  = 2;    // how wide the glow spreads

void main()
{
    vec3 hdrColor = texture(HdrBuffer, UV).rgb;
    vec3 bloom    = vec3(0.0);

    if (BloomEnabled)
    {
        vec2  texel  = 1.0 / vec2(textureSize(HdrBuffer, 0));
        float totalW = 0.0;

        // Gaussian-weighted blur of pixels above the brightness threshold
        for (int x = -blurRadius; x <= blurRadius; x++) {
            for (int y = -blurRadius; y <= blurRadius; y++) {
                vec3  s          = texture(HdrBuffer, UV + vec2(x, y) * texel).rgb;
                float brightness = dot(s, vec3(0.2126, 0.7152, 0.0722));
                if (brightness > bloomThresh) {
                    float w  = exp(-0.5 * float(x*x + y*y) / 4.0);
                    bloom   += s * w;
                    totalW  += w;
                }
            }
        }
        if (totalW > 0.0) bloom /= totalW;
    }

    vec3 combined = hdrColor + bloom * bloomStr;

    vec3 mapped = vec3(1.0) - exp(-combined * exposure);

    mapped = pow(mapped, vec3(1.0 / 2.2));

    FragColor = vec4(mapped, 1.0);
}