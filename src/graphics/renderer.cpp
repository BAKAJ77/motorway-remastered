#include <graphics/renderer.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <fstream>

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void Renderer::Init() const
{
    // Enable blending which allows for transparency rendering
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glEnable(GL_DEPTH_TEST); // Enable depth testing

    // Initialize the shaders required by the renderer
    AssetSystem::GetInstance().LoadShader("Phong-Lighting-Geometry", "shaders/common.glsl.vsh", "shaders/phong_lighting.glsl.fsh");
    AssetSystem::GetInstance().LoadShader("Basic-Geometry", "shaders/common.glsl.vsh", "shaders/basic.glsl.fsh");
}

void Renderer::Clear(ClearFlag mask, const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClear((uint32_t)mask);
}

void Renderer::Render(const Camera3D& camera, const Geometry& geometry, const SceneLighting* lighting) const
{
    // Fetch and bind the geometry shader
    const ShaderProgramPtr geometryShader = lighting ? AssetSystem::GetInstance().GetShader("Phong-Lighting-Geometry") :
        AssetSystem::GetInstance().GetShader("Basic-Geometry");

    geometryShader->Bind();

    // Assign common shader uniforms
    const Geometry::Material& material = geometry.GetMaterialData();
    const glm::mat4 modelMatrix = geometry.ComputeModelMatrix();

    geometryShader->SetUniform("v_computeLighting", lighting != nullptr);

    // Space transformation matrix uniforms
    geometryShader->SetUniformEx("v_modelMatrix", modelMatrix);
    geometryShader->SetUniformEx("v_cameraMatrix", camera.ComputeProjectionMatrix() * camera.ComputeViewMatrix());

    // Material uniforms
    geometryShader->SetUniformEx("f_material.m_diffuseColor", material.m_diffuseColor);
    geometryShader->SetUniform("f_material.m_opacity", material.m_opacity);

    geometryShader->SetUniform("f_material.m_enableDiffuseTexture", material.m_enableTextures && material.m_diffuseTexture);
    
    if (material.m_diffuseTexture)
    {
        geometryShader->SetUniform("f_material.m_diffuseTexture", 0);
        material.m_diffuseTexture->Bind(0); // Bind the diffuse texture
    }

    // Assign lighting-specific shader uniforms
    if (lighting)
    {
        geometryShader->SetUniformEx("v_normalMatrix", glm::transpose(glm::inverse(glm::mat3(modelMatrix))));
        geometryShader->SetUniformEx("f_cameraPosition", camera.GetPosition());

        // Material uniforms
        geometryShader->SetUniformEx("f_material.m_ambientColor", material.m_ambientColor);
        geometryShader->SetUniformEx("f_material.m_specularColor", material.m_specularColor);
        geometryShader->SetUniformEx("f_material.m_emissionColor", material.m_emissionColor);
        geometryShader->SetUniform("f_material.m_shininess", material.m_shininess);

        geometryShader->SetUniform("f_material.m_enableSpecularTexture", material.m_enableTextures && material.m_specularTexture);
        geometryShader->SetUniform("f_material.m_enableEmissionTexture", material.m_enableTextures && material.m_emissionTexture);

        if (material.m_specularTexture)
        {
            geometryShader->SetUniform("f_material.m_specularTexture", 1);
            material.m_specularTexture->Bind(1); // Bind the specular texture
        }

        if (material.m_emissionTexture)
        {
            geometryShader->SetUniform("f_material.m_emissionTexture", 2);
            material.m_emissionTexture->Bind(2); // Bind the emission texture
        }

        // Directional light uniforms
        geometryShader->SetUniform("f_globalLight.m_enabled", lighting->m_globalLight.m_enabled);

        geometryShader->SetUniformEx("f_globalLight.m_direction", lighting->m_globalLight.m_direction);
        geometryShader->SetUniformEx("f_globalLight.m_ambientIntensity", lighting->m_globalLight.m_ambientIntensity);
        geometryShader->SetUniformEx("f_globalLight.m_diffuseIntensity", lighting->m_globalLight.m_diffuseIntensity);
        geometryShader->SetUniformEx("f_globalLight.m_specularIntensity", lighting->m_globalLight.m_specularIntensity);

        // Point light uniforms
        geometryShader->SetUniform("f_pointLight.m_enabled", lighting->m_pointLight.m_enabled);

        geometryShader->SetUniformEx("f_pointLight.m_position", lighting->m_pointLight.m_position);
        geometryShader->SetUniformEx("f_pointLight.m_ambientIntensity", lighting->m_pointLight.m_ambientIntensity);
        geometryShader->SetUniformEx("f_pointLight.m_diffuseIntensity", lighting->m_pointLight.m_diffuseIntensity);
        geometryShader->SetUniformEx("f_pointLight.m_specularIntensity", lighting->m_pointLight.m_specularIntensity);

        geometryShader->SetUniform("f_pointLight.m_constantFactor", lighting->m_pointLight.m_constantFactor);
        geometryShader->SetUniform("f_pointLight.m_linearFactor", lighting->m_pointLight.m_linearFactor);
        geometryShader->SetUniform("f_pointLight.m_quadraticFactor", lighting->m_pointLight.m_quadraticFactor);

        // Spot light uniforms
        geometryShader->SetUniform("f_spotLight.m_enabled", lighting->m_spotLight.m_enabled);

        geometryShader->SetUniformEx("f_spotLight.m_position", lighting->m_spotLight.m_position);
        geometryShader->SetUniformEx("f_spotLight.m_direction", lighting->m_spotLight.m_direction);
        geometryShader->SetUniformEx("f_spotLight.m_ambientIntensity", lighting->m_spotLight.m_ambientIntensity);
        geometryShader->SetUniformEx("f_spotLight.m_diffuseIntensity", lighting->m_spotLight.m_diffuseIntensity);
        geometryShader->SetUniformEx("f_spotLight.m_specularIntensity", lighting->m_spotLight.m_specularIntensity);

        geometryShader->SetUniform("f_spotLight.m_constantFactor", lighting->m_spotLight.m_constantFactor);
        geometryShader->SetUniform("f_spotLight.m_linearFactor", lighting->m_spotLight.m_linearFactor);
        geometryShader->SetUniform("f_spotLight.m_quadraticFactor", lighting->m_spotLight.m_quadraticFactor);

        geometryShader->SetUniform("f_spotLight.m_innerCutOff", glm::cos(glm::radians(lighting->m_spotLight.m_innerCutOffAngle)));
        geometryShader->SetUniform("f_spotLight.m_outerCutOff", glm::cos(glm::radians(lighting->m_spotLight.m_outerCutOffAngle)));
    }

    // Bind the geometry vao
    geometry.GetVertexArray().Bind();

    // Draw the geometry
    if (geometry.GetRenderFunction() == Geometry::RenderFunction::RENDER_ARRAYS)
        glDrawArrays((uint32_t)geometry.GetPrimitiveType(), 0, geometry.GetCount());
    else if (geometry.GetRenderFunction() == Geometry::RenderFunction::RENDER_ELEMENTS)
        glDrawElements((uint32_t)geometry.GetPrimitiveType(), geometry.GetCount(), GL_UNSIGNED_INT, nullptr);
}

Renderer& Renderer::GetInstance()
{
    static Renderer instance;
    return instance;
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

Renderer::ClearFlag operator|(Renderer::ClearFlag lhs, Renderer::ClearFlag rhs)
{
    return (Renderer::ClearFlag)((uint32_t)lhs | (uint32_t)rhs);
}

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////