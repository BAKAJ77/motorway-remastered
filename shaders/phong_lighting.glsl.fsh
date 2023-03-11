#version 330 core

struct Material // Struct defining the geometry's material
{
    vec3 m_ambientColor, m_diffuseColor, m_specularColor;
    sampler2D m_diffuseTexture, m_specularTexture;
    float m_opacity, m_shininess;

    bool m_enableDiffuseTexture, m_enableSpecularTexture;
};

struct DirectionalLight // Struct defining a global light source
{
    vec3 m_direction;
    vec3 m_ambientIntensity, m_diffuseIntensity, m_specularIntensity;
};

// Inputs from the vertex shader
in vec2 f_uvCoords;
in vec3 f_fragmentPosition;
in vec3 f_normal;

// Lighting related uniforms
uniform Material f_material;
uniform DirectionalLight f_globalLight;
uniform vec3 f_cameraPosition;

// Returns the ambient lighting component computed from the global directional light
vec3 ComputeAmbientComponent(DirectionalLight globalLight, Material material)
{
    vec3 ambientColor = material.m_ambientColor;
    if (material.m_enableDiffuseTexture)
        ambientColor *= texture(material.m_diffuseTexture, f_uvCoords).rgb;

    return globalLight.m_ambientIntensity * ambientColor;
}

// Returns the diffuse lighting component from the global directional light
vec3 ComputeDiffuseComponent(DirectionalLight globalLight, Material material, vec3 normal)
{
    vec3 normalDir = normalize(normal);
    vec3 lightDir = normalize(globalLight.m_direction);

    vec3 diffuseColor = material.m_diffuseColor;
    if (material.m_enableDiffuseTexture)
        diffuseColor *= texture(material.m_diffuseTexture, f_uvCoords).rgb;

    float diffuseFactor = max(dot(-lightDir, normalDir), 0.0f);
    return globalLight.m_diffuseIntensity * diffuseColor * diffuseFactor;
}

// Returns the specular lighting component from the global directional light
vec3 ComputeSpecularComponent(DirectionalLight globalLight, Material material, vec3 normal, vec3 cameraPos)
{
    vec3 normalDir = normalize(normal);
    vec3 lightDir = normalize(globalLight.m_direction);
    vec3 reflectedLightDir = reflect(lightDir, normalDir);
    vec3 cameraDir = normalize(f_cameraPosition - f_fragmentPosition);

    vec3 specularColor = material.m_specularColor;
    if (material.m_enableSpecularTexture)
        specularColor *= texture(material.m_specularTexture, f_uvCoords).rgb;

    float specularFactor = pow(max(dot(cameraDir, reflectedLightDir), 0.0f), material.m_shininess);
    return globalLight.m_specularIntensity * specularColor * specularFactor;
}

void main()
{
    // Calculate the ambient lighting component
    vec3 ambientComponent = ComputeAmbientComponent(f_globalLight, f_material);

    // Calculate the diffuse lighting component
    vec3 diffuseComponent = ComputeDiffuseComponent(f_globalLight, f_material, f_normal);

    // Calculate the specular lighting component
    vec3 specularComponent = ComputeSpecularComponent(f_globalLight, f_material, f_normal, f_cameraPosition);

    // Finally combine the computed lighting components into one final fragment color
    gl_FragColor = vec4(ambientComponent + diffuseComponent + specularComponent,
        texture(f_material.m_diffuseTexture, f_uvCoords).a * f_material.m_opacity);
}
