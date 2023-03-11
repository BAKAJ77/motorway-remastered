#version 330 core

struct Material // Struct defining the geometry's material
{
    vec3 m_diffuseColor;
    sampler2D m_diffuseTexture;
    float m_opacity;

    bool m_enableDiffuseTexture;
};

in vec2 f_uvCoords;
uniform Material f_material;

void main()
{
    vec4 fragColor = vec4(f_material.m_diffuseColor, f_material.m_opacity);
    if (f_material.m_enableDiffuseTexture)
        fragColor *= texture(f_material.m_diffuseTexture, f_uvCoords);

    gl_FragColor = fragColor;
}
