#version 330

in vec2 fragTexCoord;

uniform sampler2D prevState;
uniform float current_time;

out vec4 finalColor;

void main() {
    // Leer estado anterior
    vec2 prevValue = texture(prevState, fragTexCoord).rg;

    // Generar un patrón de ondas concéntricas simple
    vec2 center = vec2(0.1, 0.1);
    float dist = distance(fragTexCoord, center);
    float wave = sin(dist * 20.0 - current_time * 2.0) * 0.5 + 0.5;

    // Mezclar el patrón nuevo con el estado anterior (para ver acumulación)
    float newValue = prevValue.r * 0.95 + wave * 0.05;

    finalColor = vec4(newValue, 0.0, 0.0, 1.0);
}