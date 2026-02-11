#version 330

in vec2 fragTexCoord;

uniform sampler2D prevState;  // R = height, G = velocity
uniform sampler2D depth;
uniform sampler2D prevDepth;

uniform float current_time;

out vec4 finalColor;

void main() {
    //from textures
    vec2 state = texture(prevState, fragTexCoord).rg;
    float currDepth = texture(depth, fragTexCoord).r;
    float prevD = texture(prevDepth, fragTexCoord).r;

    // read previous state
    float height = state.r;
    float velocity = state.g;

    vec2 texelSize = 1.0 / vec2(textureSize(prevState, 0));

    // laplaciano (promedio de vecinos - centro)
    float left = texture(prevState, fragTexCoord + vec2(-texelSize.x, 0)).r;
    float right = texture(prevState, fragTexCoord + vec2(texelSize.x, 0)).r;
    float up = texture(prevState, fragTexCoord + vec2(0, -texelSize.y)).r;
    float down = texture(prevState, fragTexCoord + vec2(0, texelSize.y)).r;

    float neighbors = (left + right + up + down) * 0.25;
    float laplacian = neighbors - height;

    // update speed and height
    float waveSpeed = 1.2;
    float damping = 0.97;

    velocity += laplacian * waveSpeed;
    velocity *= damping;
    height += velocity;

    height *= 0.98;

    // detect change in depth (movement)
    float change = abs(prevD - currDepth);

    if (change > 0.5) {  // threshold de movimiento
        height += sin(current_time * 5.0) * 0.1;  // impulso fuerte
    }

    height = clamp(height, -1.0, 1.0);
    velocity = clamp(velocity, -2.0, 2.0);

    finalColor = vec4(height, velocity, 0.0, 1.0);
}