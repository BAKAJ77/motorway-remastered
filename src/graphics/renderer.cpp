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
    AssetSystem::GetInstance().LoadShader("Geometry", "shaders/common.glsl.vsh", "shaders/geometry.glsl.fsh");
}

void Renderer::Clear(ClearFlag mask, const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClear((uint32_t)mask);
}

void Renderer::Render(const Camera3D& camera, const Geometry& geometry) const
{
    AssetSystem::GetInstance().GetShader("Geometry")->Bind(); // Bind the geometry shader

    // Assign the matrix shader uniforms
    AssetSystem::GetInstance().GetShader("Geometry")->SetUniformEx("v_modelMatrix", geometry.ComputeModelMatrix());
    AssetSystem::GetInstance().GetShader("Geometry")->SetUniformEx("v_cameraMatrix", camera.ComputeProjectionMatrix() *
        camera.ComputeViewMatrix());

    // Assign the material shader uniforms
    const Geometry::Material& material = geometry.GetMaterialData();

    AssetSystem::GetInstance().GetShader("Geometry")->SetUniformEx("f_material.m_diffuseColor", material.m_diffuseColor);
    AssetSystem::GetInstance().GetShader("Geometry")->SetUniform("f_material.m_enableDiffuseTexture", material.m_enableTextures);

    if (material.m_diffuseTexture)
    {
        AssetSystem::GetInstance().GetShader("Geometry")->SetUniform("f_material.m_diffuseTexture", 0);
        material.m_diffuseTexture->Bind(0); // Bind the diffuse texture
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