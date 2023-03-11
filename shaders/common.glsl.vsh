#version 330 core
layout (location = 0) in vec3 v_vertexCoords;
layout (location = 1) in vec2 v_uvCoords;
layout (location = 2) in vec3 v_normal;

out vec2 f_uvCoords;
out vec3 f_fragmentPosition;
out vec3 f_normal;

uniform mat4 v_modelMatrix, v_cameraMatrix;
uniform mat3 v_normalMatrix;

uniform bool v_computeLighting;

void main()
{
    // Forward UV coords to the fragment shader
    f_uvCoords = v_uvCoords;

    // Forward the fragment position and normal direction to the fragment shader
    // Also, note that lighting calculations will be done in world-space
    if (v_computeLighting)
    {
        f_normal = v_normalMatrix * v_normal;
        f_fragmentPosition = vec3(v_modelMatrix * vec4(v_vertexCoords, 1.0f));
    }

    // Compute the final vertex position
    gl_Position = v_cameraMatrix * v_modelMatrix * vec4(v_vertexCoords, 1.0f);
}
