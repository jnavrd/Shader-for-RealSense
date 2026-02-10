#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D texture0;
uniform vec4 colDiffuse;

uniform float current_time;
uniform float min_range;
uniform float max_range;

// Output fragment color
out vec4 finalColor;

void main()
{
    // Texel fetching from texture sampler
    vec4 texel = texture(texture0, fragTexCoord);

    vec3 color = vec3(1,0,1);
    finalColor = vec4(color, 1.0);
}