#version 330 core

struct Material
{
    vec4 m_diffuseColor;
    sampler2D m_diffuseTexture;
    bool m_enableTextures;
};

in vec2 f_uvCoords;
uniform Material f_material;

void main()
{
    vec4 finalColor = vec4(1.0f);

    if (f_material.m_enableTextures)
        finalColor = texture(f_material.m_diffuseTexture, f_uvCoords) * f_material.m_diffuseColor;
    else
        finalColor = f_material.m_diffuseColor;

    gl_FragColor = finalColor;
}
