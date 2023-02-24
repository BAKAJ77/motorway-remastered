#version 330 core
layout (location = 0) in vec3 vertexCoords;

void main()
{
    gl_Position = vec4(vertexCoords, 1.0f);
}
