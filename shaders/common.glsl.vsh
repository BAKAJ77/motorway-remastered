#version 330 core
layout (location = 0) in vec3 vertexCoords;
layout (location = 1) in vec2 uvCoords;

out VSH_OUT
{
    vec2 uvCoords;
} vshOut;

void main()
{
    vshOut.uvCoords = uvCoords;
    gl_Position = vec4(vertexCoords, 1.0f);
}
