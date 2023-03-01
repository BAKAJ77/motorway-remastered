#version 330 core
layout (location = 0) in vec3 vertexCoords;
layout (location = 1) in vec2 uvCoords;

uniform mat4 modelMatrix, cameraMatrix;

out VSH_OUT
{
    vec2 uvCoords;
} vshOut;

void main()
{
    vshOut.uvCoords = uvCoords;
    gl_Position = cameraMatrix * modelMatrix * vec4(vertexCoords, 1.0f);
}
