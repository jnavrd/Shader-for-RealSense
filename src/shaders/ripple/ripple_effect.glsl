#version 330

// Input vertex attributes (from vertex shader)
in vec2 fragTexCoord;
in vec4 fragColor;

// Input uniform values
uniform sampler2D depthData;
uniform sampler2D rippleData;
//uniform vec4 colDiffuse;

//uniform float current_time;
uniform float min_range;
uniform float max_range;

// Output fragment color
out vec4 finalColor;

float normalize_depth(float depth_value)
{
    float normalized = (depth_value - min_range) / (max_range - min_range);

    return clamp(normalized, 0.0, 1.0);
}

//unused, kernel
vec2[9] set_offsets(vec2 step_size)
{

    vec2 offsets[9] = vec2[](
    vec2(-step_size.x,  -step_size.y),
    vec2( 0.0f,         -step_size.y),
    vec2( step_size.x,  -step_size.y),
    vec2(-step_size.x,   0.0f),
    vec2( 0.0f,          0.0f),
    vec2( step_size.x,   0.0f),
    vec2(-step_size.x,  +step_size.y),
    vec2( 0.0f,         +step_size.y),
    vec2( step_size.x,   step_size.y)
    );

    return offsets;
}

// 2D Random
float random (vec2 st) {
    return fract(sin(dot(st.xy,
    vec2(12.9898,78.233)))
    * 43758.5453123);
}

float noise (vec2 st) {

    //esquina inf izquierda
    vec2 i = floor(st); // floor:  integrer mas grande que es menor o igual al input.
    //pos dentro de la celda
    vec2 f = fract(st); // x - floor(x) rango [ 0.0, 1.0 )

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    // Smooth Interpolation
    // Cubic Hermine Curve.  Same as SmoothStep()
    vec2 u = f*f*(3.0-2.0*f);
    //vec2 u = smoothstep(0.,1.,f);

    // Mix 4 coorners percentages
    return mix(a, b, u.x) +
    (c - a)* u.y * (1.0 - u.x) +
    (d - b) * u.x * u.y;
}

float fbm(vec2 st) {
    float value = 0.0;
    float amplitude = 1.2;

    for (int i = 0; i < 3; i++) {
        value += amplitude * noise(st);
        st *= 2.0;        // duplica la frecuencia (constante, no oscilante)
        amplitude *= 0.5; // reduce la amplitud a la mitad
    }
    return value;
}

void main()
{
    // Texel fetching from texture sampler
    vec4 texel = texture(depthData, fragTexCoord);
    vec4 rippleState = texture(rippleData, fragTexCoord);

    vec2 texelSize = 1.0 / vec2(textureSize(rippleData, 0));

    //raw depth value value in meters
    float depth_value = texel.r;
    float gray_scale = normalize_depth(depth_value);

    //UNUSED, kernel for edge detection
    /*ivec2 texture_size = textureSize(depthData, 0);
    vec2 step_size = 1.0/texture_size; //textel size
    vec2 offsets[9] = set_offsets(step_size);*/

    //depth visualization
    vec3 color = vec3(gray_scale);

    //riples
    float heightLeft = texture(rippleData, fragTexCoord + vec2(-texelSize.x, 0.0)).r;
    float heightRight = texture(rippleData, fragTexCoord + vec2(texelSize.x, 0.0)).r;
    float heightUp = texture(rippleData, fragTexCoord + vec2(0.0, -texelSize.y)).r;
    float heightDown = texture(rippleData, fragTexCoord + vec2(0.0, texelSize.y)).r;


    vec2 rippleGradient = vec2(
        heightRight - heightLeft,
        heightDown - heightUp
    );

    // Background

    float ripple = 0.0;
    float rippleHeight = rippleState.r;

    float distortionStrength = 1.6; // ajusta este valor
    vec2 distortedUV = fragTexCoord;
    float n = fbm((distortedUV * 5.0) + rippleHeight * 2.0);

    //float n = fbm(fragTexCoord*5.0);
    n = floor(n * 6.0) / 6.0;
    vec3 background = vec3(0.35, 0.55, 0.9)*n;

    float background_threshold = 0.75;


    if (depth_value > background_threshold) {
        color = background;
    }


    finalColor = vec4(color, 1.0);
}