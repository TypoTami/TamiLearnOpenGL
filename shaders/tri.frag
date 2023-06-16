#version 330 core
out vec4 FragColor;

in vec3 ourColour;
in vec2 TexCoord;

uniform sampler2D ourTexture;

void main()
{
//    FragColor = vec4(ourColour, 1.0f);
    FragColor = texture(ourTexture, TexCoord) * vec4(ourColour, 1.0f);
}