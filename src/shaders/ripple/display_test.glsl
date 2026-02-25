#version 330

in vec2 fragTexCoord;

uniform sampler2D rippleData;

out vec4 finalColor;

void main() {
    float ripple = texture(rippleData, fragTexCoord).r;

    // Visualizar en escala de grises
    vec3 color = vec3(ripple);

    finalColor = vec4(color, 1.0);
}