#version 330 core
layout (location = 0) in vec3 v_vertexCoords;
layout (location = 1) in vec2 v_uvCoords;

uniform mat4 v_modelMatrix, v_cameraMatrix;
out vec2 f_uvCoords;

void main()
{
    f_uvCoords = v_uvCoords;
    gl_Position = v_cameraMatrix * v_modelMatrix * vec4(v_vertexCoords, 1.0f);
}
