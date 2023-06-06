#version 330 core
layout (location = 0) in vec3 aPos; // The posisition variable has attribute position 0

out vec4 vertexColour;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vertexColour = vec4(0.5, 0.0, 0.0, 1.0);
}