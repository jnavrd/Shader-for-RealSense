#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform float min_range;
uniform float max_range;

// Output fragment color
out vec4 finalColor;

float normalize_depth(float depth_value)
{
    float normalized = (depth_value - min_range) / (max_range - min_range);

    return clamp(normalized, 0.0, 1.0);
}

void main()
{
    // Texel fetching from texture sampler
    vec4 texel = texture(texture0, fragTexCoord);

    //raw value in float
    float depth_value = texel.r; // PIXELFORMAT_UNCOMPRESSED_R32, 32 bpp (1 channel - float)
    float gray_scale = normalize_depth(depth_value);

    vec3 color = vec3(gray_scale);
    finalColor = vec4(color, 1.0);
}