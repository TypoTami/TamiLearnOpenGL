#version 330 core
out vec4 FragColor;
in vec3 ourColour;

void main()
{
    ivec3 intCol = ivec3(ourColour * 10.0f);
    vec3 fltCol = vec3(intCol) / 10.0f;

    FragColor = vec4(fltCol, 1.0f);
}