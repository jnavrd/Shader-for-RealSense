#version 330

in vec2 fragTexCoord;

uniform sampler2D prevState;
uniform float current_time;

out vec4 finalColor;

void main() {
    // Test simple: escribir un patrón fijo
    vec2 center = vec2(0.5, 0.5);
    float dist = distance(fragTexCoord, center);

    float height = 0.0;
    float velocity = 0.0;

    if (dist < 0.1) {
        height = 0.5;  // círculo fijo en el centro
    }

    finalColor = vec4(height, velocity, 0.0, 1.0);
}