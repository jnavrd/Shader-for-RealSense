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
    float waveSpeed = 1.2;
    float damping = 0.995;  // solo 0.5% de pérdida por frame

    velocity += laplacian * waveSpeed;
    velocity *= damping;
    height += velocity;

    // Detectar impulso por movimiento
    float currDepth = texture(depth, fragTexCoord).r;
    float prevD = texture(prevDepth, fragTexCoord).r;
    float change = abs(prevD - currDepth);

    if (change > 0.1) {  // threshold de movimiento
        height += sin(current_time * 5.0) * 0.1;  // impulso fuerte
    }

    velocity *= damping;
    height += velocity;

    finalColor = vec4(height, velocity, 0.0, 1.0);
}