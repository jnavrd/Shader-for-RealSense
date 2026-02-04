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

    //raw depth value value in meters
    float depth_value = texel.r;
    float gray_scale = normalize_depth(depth_value);

    ivec2 texture_size = textureSize(texture0, 0);
    vec2 step_size = 1.0/texture_size; //textel size

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

    vec3 color = vec3(gray_scale);
    vec3 backgound = vec3(0,0,1);
    float background_threshold = 0.75;
   // int close_neighbors; //amount of pixels close to camera vs far to be considered edge of object

    if(depth_value > background_threshold) //if i am background
    {
        color = backgound;
        for(int i = 0; i < 9; i++) {
            vec2 neighbor_coord = fragTexCoord + offsets[i];
            float neighbor_depth = texture(texture0, neighbor_coord).r;
            if(depth_value - neighbor_depth > background_threshold) //edge
            {
                color = vec3(1,0,0);
            }
        }
    }

    finalColor = vec4(color, 1.0);
}