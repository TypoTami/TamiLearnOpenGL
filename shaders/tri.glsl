#version 330 core
layout (location = 0) in vec3 aPos;     // The posisition variable has attribute position 0
layout (location = 1) in vec3 aColour;  // The colour varable has attribute position 1
layout (location = 2) in vec2 aTexCoord;// The texture coordinates has attribute position 2

out vec3 ourColour;
out vec2 TexCoord;

uniform mat4 transform;

void main()
{
    gl_Position = transform * vec4(aPos, 1.0f);
    ourColour = aColour;
    TexCoord = aTexCoord;
}