#version 450

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

layout(binding = 1) uniform sampler2D texSampler;

void main() {
    // use colours to debug texture coordinates
    // The green channel represents the horizontal coordinates and 
    // the red channel the vertical coordinates. The black and yellow
    // corners confirm that the texture coordinates are correctly
    // interpolated from 0,  0 to 1, 1 across the square. Visualizing 
    // data using colors is the shader programming equivalent of printf 
    // debugging, for lack of a better option!
    // outColor = vec4(fragTexCoord, 0.0, 1.0);
    outColor = texture(texSampler, fragTexCoord);

    // modify texture coordinates
    // outColor = texture(texSampler, fragTexCoord * 4.0);

    // manipulate texture colours using the vertex colours
    // outColor = vec4(fragColor * texture(texSampler, fragTexCoord).rgb, 1.0);

    // pretty colours
    // vec3 color = vec3(fragTexCoord.x, fragTexCoord.y, 1.0);
    // vec3 color = vec3(fragTexCoord.x, fragTexCoord.y, 0.0);
    // outColor = vec4(color, 1.0);

    // vec4 texColor = texture(texSampler, fragTexCoord);
    // outColor = vec4(texColor.rgb * color, 1.0);
}