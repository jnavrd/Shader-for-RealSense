#version 330

in vec2 fragTexCoord;

uniform sampler2D prevState;  // R = altura, G = velocidad
uniform sampler2D depth;
uniform sampler2D prevDepth;

uniform float current_time;

out vec4 finalColor;

void main() {
    vec2 state = texture(prevState, fragTexCoord).rg;

    vec2 texelSize = 1.0 / vec2(textureSize(prevState, 0));

    // Leer estado anterior
    float height = state.r;
    float velocity = state.g;

    // Calcular laplaciano (promedio de vecinos - centro)
    float left = texture(prevState, fragTexCoord + vec2(-texelSize.x, 0)).r;
    float right = texture(prevState, fragTexCoord + vec2(texelSize.x, 0)).r;
    float up = texture(prevState, fragTexCoord + vec2(0, -texelSize.y)).r;
    float down = texture(prevState, fragTexCoord + vec2(0, texelSize.y)).r;

    float neighbors = (left + right + up + down) * 0.25;
    float laplacian = neighbors - height;

    // Física: actualizar velocidad y altura
    float waveSpeed = 0.5;
    float damping = 0.98;

    velocity += laplacian * waveSpeed;
    velocity *= damping;
    height += velocity;

    // Detectar impulso por movimiento
    float currDepth = texture(depth, fragTexCoord).r;
    float prevD = texture(prevDepth, fragTexCoord).r;
    float change = abs(prevD - currDepth);

    if (change > 0.05) {  // threshold de movimiento
        height += 0.5;  // impulso fuerte
    }
    vec2 center = vec2(0.5, 0.5);
    if (distance(fragTexCoord, center) < 0.05) {
        height += sin(current_time * 5.0) * 0.1;  // pulsa en el centro
    }

    finalColor = vec4(height, velocity, 0.0, 1.0);
}