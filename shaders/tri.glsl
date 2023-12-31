#version 330 core
layout (location = 0) in vec3 aPos;     // The posisition variable has attribute position 0
layout (location = 1) in vec2 aTexCoord;// The texture coordinates has attribute position 1

out vec2 TexCoord;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    // Note that matrix multiplicatoin is read right to left
    gl_Position = projection * view * model * vec4(aPos, 1.0f);
    TexCoord = aTexCoord;
}