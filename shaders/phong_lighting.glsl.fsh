#version 330 core

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

struct Material // Struct defining the geometry's material
{
    vec3 m_ambientColor, m_diffuseColor, m_specularColor, m_emissionColor;
    sampler2D m_diffuseTexture, m_specularTexture, m_emissionTexture;
    float m_opacity, m_shininess;

    bool m_enableDiffuseTexture, m_enableSpecularTexture, m_enableEmissionTexture;
};

struct DirectionalLight // Struct defining a global-light source
{
    vec3 m_direction;
    vec3 m_ambientIntensity, m_diffuseIntensity, m_specularIntensity;

    bool m_enabled;
};

struct PointLight // Struct defining a point-light source
{
    vec3 m_position;
    vec3 m_ambientIntensity, m_diffuseIntensity, m_specularIntensity;
    float m_constantFactor, m_linearFactor, m_quadraticFactor;

    bool m_enabled;
};

struct SpotLight // Struct defining a spot-light source
{
    vec3 m_position, m_direction;
    vec3 m_ambientIntensity, m_diffuseIntensity, m_specularIntensity;
    float m_constantFactor, m_linearFactor, m_quadraticFactor, m_innerCutOff, m_outerCutOff;

    bool m_enabled;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Inputs from the vertex shader
in vec2 f_uvCoords;
in vec3 f_fragmentPosition;
in vec3 f_normal;

// Lighting related uniforms
uniform Material f_material;
uniform DirectionalLight f_globalLight;
uniform PointLight f_pointLight;
uniform SpotLight f_spotLight;

uniform vec3 f_cameraPosition;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Returns the computed light intensity attenuation factor
float ComputeAttenuationFactor(vec3 lightPos, vec3 fragPos, float constantFactor, float linearFactor,
    float quadraticFactor)
{
    float lightDistance = length(lightPos - fragPos);
    return 1.0f / (constantFactor + (linearFactor * lightDistance) +
        (quadraticFactor * (lightDistance * lightDistance)));
}

// Returns the computed light intensity from a spot-light towards the given fragment
float ComputeSpotLightFragmentIntensity(SpotLight spotLight, vec3 fragPos)
{
    vec3 lightDir = normalize(fragPos - spotLight.m_position);
    float theta = dot(lightDir, normalize(spotLight.m_direction));
    float cosineDiff = spotLight.m_innerCutOff - spotLight.m_outerCutOff;

    return clamp((theta - spotLight.m_outerCutOff) / cosineDiff, 0.0f, 1.0f);
}

// Returns the computed emission component from the given material
vec3 ComputeEmissionComponent(Material material)
{
    vec3 emissionColor = vec3(0.0f);
    if (material.m_enableEmissionTexture)
        emissionColor = texture(material.m_emissionTexture, f_uvCoords).rgb * material.m_emissionColor;

    return emissionColor;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Returns the ambient lighting component computed from the given global directional light
vec3 ComputeAmbientComponent(DirectionalLight globalLight, Material material)
{
    // Get the material's ambient color
    vec3 ambientColor = material.m_ambientColor;
    if (material.m_enableDiffuseTexture)
        ambientColor *= texture(material.m_diffuseTexture, f_uvCoords).rgb;

    // Combine all the calculations to get the final ambient component
    return globalLight.m_ambientIntensity * ambientColor;
}

// Returns the ambient lighting component computed from the given point-light
vec3 ComputeAmbientComponent(PointLight pointLight, Material material, vec3 fragPos)
{
    // Get the material's ambient color
    vec3 ambientColor = material.m_ambientColor;
    if (material.m_enableDiffuseTexture)
        ambientColor *= texture(material.m_diffuseTexture, f_uvCoords).rgb;

    // Calculate the attenuation factor
    float attenuationFactor = ComputeAttenuationFactor(pointLight.m_position, fragPos,
        pointLight.m_constantFactor, pointLight.m_linearFactor, pointLight.m_quadraticFactor);

    // Combine all the calculations to get the final ambient component
    return attenuationFactor * pointLight.m_ambientIntensity * ambientColor;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Returns the diffuse lighting component from the given global directional light
vec3 ComputeDiffuseComponent(DirectionalLight globalLight, Material material, vec3 normal)
{
    // Get the material's diffuse color
    vec3 diffuseColor = material.m_diffuseColor;
    if (material.m_enableDiffuseTexture)
        diffuseColor *= texture(material.m_diffuseTexture, f_uvCoords).rgb;

    // Calculate the diffuse factor
    vec3 normalDir = normalize(normal);
    vec3 lightDir = normalize(globalLight.m_direction);
    float diffuseFactor = max(dot(-lightDir, normalDir), 0.0f);

    // Combine all the calculations to get the final diffuse component
    return globalLight.m_diffuseIntensity * diffuseColor * diffuseFactor;
}

// Returns the diffuse lighting component from the given point-light
vec3 ComputeDiffuseComponent(PointLight pointLight, Material material, vec3 normal, vec3 fragPos)
{
    // Get the material's diffuse color
    vec3 diffuseColor = material.m_diffuseColor;
    if (material.m_enableDiffuseTexture)
        diffuseColor *= texture(material.m_diffuseTexture, f_uvCoords).rgb;

    // Calculate the diffuse factor
    vec3 normalDir = normalize(normal);
    vec3 lightDir = normalize(fragPos - pointLight.m_position);
    float diffuseFactor = max(dot(-lightDir, normalDir), 0.0f);

    // Calculate the attenuation factor
    float attenuationFactor = ComputeAttenuationFactor(pointLight.m_position, fragPos,
        pointLight.m_constantFactor, pointLight.m_linearFactor, pointLight.m_quadraticFactor);

    // Combine all the calculations to get the final diffuse component
    return attenuationFactor * pointLight.m_diffuseIntensity * diffuseColor * diffuseFactor;
}

// Returns the diffuse lighting component from the given spot-light
vec3 ComputeDiffuseComponent(SpotLight spotLight, Material material, vec3 normal, vec3 fragPos)
{
    // Get the material's diffuse color
    vec3 diffuseColor = material.m_diffuseColor;
    if (material.m_enableDiffuseTexture)
        diffuseColor *= texture(material.m_diffuseTexture, f_uvCoords).rgb;

    // Calculate the diffuse factor
    vec3 normalDir = normalize(normal);
    vec3 lightDir = normalize(fragPos - spotLight.m_position);
    float diffuseFactor = max(dot(-lightDir, normalDir), 0.0f);

    // Calculate the light intensity
    float lightIntensity = ComputeSpotLightFragmentIntensity(spotLight, fragPos);

    // Calculate the attenuation factor
    float attenuationFactor = ComputeAttenuationFactor(spotLight.m_position, fragPos,
        spotLight.m_constantFactor, spotLight.m_linearFactor, spotLight.m_quadraticFactor);

    // Combine all the calculations to get the final diffuse component
    return lightIntensity * attenuationFactor * spotLight.m_diffuseIntensity * diffuseColor * diffuseFactor;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

// Returns the specular lighting component from the given global directional light
vec3 ComputeSpecularComponent(DirectionalLight globalLight, Material material, vec3 normal, vec3 cameraPos)
{
    // Get the material's specular color
    vec3 specularColor = material.m_specularColor;
    if (material.m_enableSpecularTexture)
        specularColor *= texture(material.m_specularTexture, f_uvCoords).rgb;

    // Calculate the specular factor
    vec3 normalDir = normalize(normal);
    vec3 lightDir = normalize(globalLight.m_direction);
    vec3 reflectedLightDir = reflect(lightDir, normalDir);
    vec3 cameraDir = normalize(f_cameraPosition - f_fragmentPosition);

    float specularFactor = pow(max(dot(cameraDir, reflectedLightDir), 0.0f), material.m_shininess);

    // Combine all the calculations to get the final specular component
    return globalLight.m_specularIntensity * specularColor * specularFactor;
}

// Returns the specular lighting component from the given point-light
vec3 ComputeSpecularComponent(PointLight pointLight, Material material, vec3 normal, vec3 cameraPos, vec3 fragPos)
{
    // Get the material's specular color
    vec3 specularColor = material.m_specularColor;
    if (material.m_enableSpecularTexture)
        specularColor *= texture(material.m_specularTexture, f_uvCoords).rgb;

    // Calculate the specular factor
    vec3 normalDir = normalize(normal);
    vec3 lightDir = normalize(fragPos - pointLight.m_position);
    vec3 reflectedLightDir = reflect(lightDir, normalDir);
    vec3 cameraDir = normalize(f_cameraPosition - f_fragmentPosition);

    float specularFactor = pow(max(dot(cameraDir, reflectedLightDir), 0.0f), material.m_shininess);

    // Calculate the attenuation factor
    float attenuationFactor = ComputeAttenuationFactor(pointLight.m_position, fragPos,
        pointLight.m_constantFactor, pointLight.m_linearFactor, pointLight.m_quadraticFactor);

    // Combine all the calculations to get the final specular component
    return attenuationFactor * pointLight.m_specularIntensity * specularColor * specularFactor;
}

// Returns the specular lighting component from the given spot-light
vec3 ComputeSpecularComponent(SpotLight spotLight, Material material, vec3 normal, vec3 cameraPos, vec3 fragPos)
{
    // Get the material's specular color
    vec3 specularColor = material.m_specularColor;
    if (material.m_enableSpecularTexture)
        specularColor *= texture(material.m_specularTexture, f_uvCoords).rgb;

    // Calculate the specular factor
    vec3 normalDir = normalize(normal);
    vec3 lightDir = normalize(fragPos - spotLight.m_position);
    vec3 reflectedLightDir = reflect(lightDir, normalDir);
    vec3 cameraDir = normalize(f_cameraPosition - f_fragmentPosition);

    float specularFactor = pow(max(dot(cameraDir, reflectedLightDir), 0.0f), material.m_shininess);

    // Calculate the light intensity
    float lightIntensity = ComputeSpotLightFragmentIntensity(spotLight, fragPos);

    // Calculate the attenuation factor
    float attenuationFactor = ComputeAttenuationFactor(spotLight.m_position, fragPos,
        spotLight.m_constantFactor, spotLight.m_linearFactor, spotLight.m_quadraticFactor);

    // Combine all the calculations to get the final specular component
    return lightIntensity * attenuationFactor * spotLight.m_specularIntensity * specularColor * specularFactor;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

void main()
{
    ////////////////////////////////////// PHONG-LIGHTING COMPUTATION ///////////////////////////////////////

    vec3 ambientComponent = vec3(0.0f), diffuseComponent = vec3(0.0f), specularComponent = vec3(0.0f);

    // Compute the ambient lighting component
    if (f_globalLight.m_enabled)
        ambientComponent += ComputeAmbientComponent(f_globalLight, f_material);

    if (f_pointLight.m_enabled)
        ambientComponent += ComputeAmbientComponent(f_pointLight, f_material, f_fragmentPosition);

    // Compute the diffuse lighting component
    if (f_globalLight.m_enabled)
        diffuseComponent += ComputeDiffuseComponent(f_globalLight, f_material, f_normal);

    if (f_pointLight.m_enabled)
        diffuseComponent += ComputeDiffuseComponent(f_pointLight, f_material, f_normal, f_fragmentPosition);

    if (f_spotLight.m_enabled)
        diffuseComponent += ComputeDiffuseComponent(f_spotLight, f_material, f_normal, f_fragmentPosition);

    // Compute the specular lighting component
    if (f_globalLight.m_enabled)
        specularComponent += ComputeSpecularComponent(f_globalLight, f_material, f_normal, f_cameraPosition);

    if (f_pointLight.m_enabled)
        specularComponent += ComputeSpecularComponent(f_pointLight, f_material, f_normal, f_cameraPosition,
            f_fragmentPosition);

    if (f_spotLight.m_enabled)
        specularComponent += ComputeSpecularComponent(f_spotLight, f_material, f_normal, f_cameraPosition,
            f_fragmentPosition);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////

    // Compute the emission component
    vec3 emissionComponent = ComputeEmissionComponent(f_material);

    // Finally combine the computed lighting components into one final fragment color
    gl_FragColor = vec4(ambientComponent + diffuseComponent + specularComponent + emissionComponent,
        texture(f_material.m_diffuseTexture, f_uvCoords).a * f_material.m_opacity);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
