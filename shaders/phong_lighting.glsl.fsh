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

// Returns the sampled material ambient color.
vec3 SampleMaterialAmbientComponent(Material material);

// Returns the sampled material diffuse color.
vec3 SampleMaterialDiffuseComponent(Material material);

// Returns the sampled material specular color.
vec3 SampleMaterialSpecularComponent(Material material);

// Returns the sampled material emission color.
vec3 SampleMaterialEmissionComponent(Material material);

// Returns the computed attenuation factor for the light intensity at the specified fragment position.
float ComputeAttenuationFactor(vec3 lightPos, vec3 fragmentPos, float constantFactor, float linearFactor, float quadraticFactor);

// Returns the computed color impact that the specified global-light has on the current fragment.
vec3 ComputeLightingImpact(DirectionalLight light, vec3 materialAmbient, vec3 materialDiffuse, vec3 materialSpecular,
    float materialShininess, vec3 normalDir, vec3 cameraDir);

// Returns the computed color impact that the specified point-light has on the current fragment.
vec3 ComputeLightingImpact(PointLight light, vec3 materialAmbient, vec3 materialDiffuse, vec3 materialSpecular, float materialShininess,
    vec3 normalDir, vec3 cameraDir, vec3 fragmentPos);

// Returns the computed color impact that the specified spot-light has on the current fragment.
vec3 ComputeLightingImpact(SpotLight light, vec3 materialAmbient, vec3 materialDiffuse, vec3 materialSpecular, float materialShininess,
    vec3 normalDir, vec3 cameraDir, vec3 fragmentPos);

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

void main()
{
    vec3 finalFragColor = vec3(0.0f);

    // Calculate the normal and camera directional vectors
    vec3 normalDirection = normalize(f_normal);
    vec3 cameraDirection = normalize(f_cameraPosition - f_fragmentPosition);

    // Sample the material lighting component colors
    vec3 materialAmbient = SampleMaterialAmbientComponent(f_material);
    vec3 materialDiffuse = SampleMaterialDiffuseComponent(f_material);
    vec3 materialSpecular = SampleMaterialSpecularComponent(f_material);
    vec3 materialEmission = SampleMaterialEmissionComponent(f_material);

    // Compute the global-light color impact
    if (f_globalLight.m_enabled)
    {
        finalFragColor += ComputeLightingImpact(f_globalLight, materialAmbient, materialDiffuse, materialSpecular,
            f_material.m_shininess, normalDirection, cameraDirection);
    }

    // Compute the point-light(s) color impact
    if (f_pointLight.m_enabled)
    {
            finalFragColor += ComputeLightingImpact(f_pointLight, materialAmbient, materialDiffuse, materialSpecular,
                f_material.m_shininess, normalDirection, cameraDirection, f_fragmentPosition);
    }

    // Compute the spot-light(s) color impact
    if (f_spotLight.m_enabled)
    {
            finalFragColor += ComputeLightingImpact(f_spotLight, materialAmbient, materialDiffuse, materialSpecular,
                f_material.m_shininess, normalDirection, cameraDirection, f_fragmentPosition);
    }

    // Apply the sampled emission material color onto the final fragment color
    finalFragColor += materialEmission;

    // output the final fragment color
    gl_FragColor = vec4(finalFragColor, texture(f_material.m_diffuseTexture, f_uvCoords).a * f_material.m_opacity);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

vec3 SampleMaterialAmbientComponent(Material material)
{
    vec3 ambientColor = material.m_ambientColor;
    if (material.m_enableDiffuseTexture)
        ambientColor *= texture(material.m_diffuseTexture, f_uvCoords).rgb;

    return ambientColor;
}

vec3 SampleMaterialDiffuseComponent(Material material)
{
    vec3 diffuseColor = material.m_diffuseColor;
    if (material.m_enableDiffuseTexture)
        diffuseColor *= texture(material.m_diffuseTexture, f_uvCoords).rgb;

    return diffuseColor;
}

vec3 SampleMaterialSpecularComponent(Material material)
{
    vec3 specularColor = material.m_specularColor;
    if (material.m_enableSpecularTexture)
        specularColor *= texture(material.m_specularTexture, f_uvCoords).rgb;

    return specularColor;
}

vec3 SampleMaterialEmissionComponent(Material material)
{
    vec3 emissionColor = vec3(0.0f);
    if (material.m_enableEmissionTexture)
        emissionColor = texture(material.m_emissionTexture, f_uvCoords).rgb * material.m_emissionColor;

    return emissionColor;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////

float ComputeAttenuationFactor(vec3 lightPos, vec3 fragmentPos, float constantFactor, float linearFactor, float quadraticFactor)
{
    float lightDistance = length(lightPos - fragmentPos);
    return 1.0f / (constantFactor + (linearFactor * lightDistance) + (quadraticFactor * (lightDistance * lightDistance)));
}

vec3 ComputeLightingImpact(DirectionalLight light, vec3 materialAmbient, vec3 materialDiffuse, vec3 materialSpecular,
    float materialShininess, vec3 normalDir, vec3 cameraDir)
{
    // Calculate the ambient component
    vec3 ambientComponent = materialAmbient * light.m_ambientIntensity;

    // Calculate the diffuse component
    vec3 lightDirection = normalize(-light.m_direction);
    float diffuseFactor = max(dot(lightDirection, normalDir), 0.0f);
    vec3 diffuseComponent = materialDiffuse * light.m_diffuseIntensity * diffuseFactor;

    // Calculate the specular component
    vec3 reflectedLightDir = reflect(-lightDirection, normalDir);
    float specularFactor = pow(max(dot(reflectedLightDir, cameraDir), 0.0f), materialShininess);
    vec3 specularComponent = materialSpecular * light.m_specularIntensity * specularFactor;

    // Return the combined components
    return ambientComponent + diffuseComponent + specularComponent;
}

vec3 ComputeLightingImpact(PointLight light, vec3 materialAmbient, vec3 materialDiffuse, vec3 materialSpecular,
    float materialShininess, vec3 normalDir, vec3 cameraDir, vec3 fragmentPos)
{
    // Calculate the ambient component
    vec3 ambientComponent = materialAmbient * light.m_ambientIntensity;

    // Calculate the diffuse component
    vec3 lightDirection = normalize(light.m_position - fragmentPos);
    float diffuseFactor = max(dot(lightDirection, normalDir), 0.0f);
    vec3 diffuseComponent = materialDiffuse * light.m_diffuseIntensity * diffuseFactor;

    // Calculate the specular component
    vec3 reflectedLightDir = reflect(-lightDirection, normalDir);
    float specularFactor = pow(max(dot(reflectedLightDir, cameraDir), 0.0f), materialShininess);
    vec3 specularComponent = materialSpecular * light.m_specularIntensity * specularFactor;

    // Calculate the attenuation intensity factor
    float attenuationFactor = ComputeAttenuationFactor(light.m_position, fragmentPos, light.m_constantFactor, light.m_linearFactor,
        light.m_quadraticFactor);

    // Return the combined components
    return (ambientComponent + diffuseComponent + specularComponent) * attenuationFactor;
}

vec3 ComputeLightingImpact(SpotLight light, vec3 materialAmbient, vec3 materialDiffuse, vec3 materialSpecular, float materialShininess,
    vec3 normalDir, vec3 cameraDir, vec3 fragmentPos)
{
    // Calculate the ambient component
    vec3 ambientComponent = materialAmbient * light.m_ambientIntensity;

    // Calculate the diffuse component
    vec3 lightDirection = normalize(light.m_position - fragmentPos);
    float diffuseFactor = max(dot(lightDirection, normalDir), 0.0f);
    vec3 diffuseComponent = materialDiffuse * light.m_diffuseIntensity * diffuseFactor;

    // Calculate the specular component
    vec3 reflectedLightDir = reflect(-lightDirection, normalDir);
    float specularFactor = pow(max(dot(reflectedLightDir, cameraDir), 0.0f), materialShininess);
    vec3 specularComponent = materialSpecular * light.m_specularIntensity * specularFactor;

    // Calculate the light intensity from the spot-light to the fragment
    vec3 lightRayDirection = normalize(fragmentPos - light.m_position);
    float theta = dot(lightRayDirection, normalize(light.m_direction));
    float interpolationValue = (theta - light.m_outerCutOff) / (light.m_innerCutOff - light.m_outerCutOff);

    float lightRayIntensity = smoothstep(0.0f, 1.0f, interpolationValue);

    // Calculate the attenuation intensity factor
    float attenuationFactor = ComputeAttenuationFactor(light.m_position, fragmentPos, light.m_constantFactor, light.m_linearFactor,
        light.m_quadraticFactor);

    // Return the combined components
    return (ambientComponent + diffuseComponent + specularComponent) * lightRayIntensity * attenuationFactor;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
