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
    ShaderSystem::GetInstance().Load("Geometry", { "shaders/common.glsl.vsh", "shaders/geometry.glsl.fsh" });
}

void Renderer::Clear(ClearFlag mask, const glm::vec4& color)
{
    glClearColor(color.r, color.g, color.b, color.a);
    glClear((uint32_t)mask);
}

void Renderer::Render(const Camera3D& camera, const Geometry& geometry) const
{
    ShaderSystem::GetInstance().GetShader("Geometry")->Bind(); // Bind the geometry shader

    // Assign the matrix shader uniforms
    ShaderSystem::GetInstance().GetShader("Geometry")->SetUniformEx("v_modelMatrix", geometry.ComputeModelMatrix());
    ShaderSystem::GetInstance().GetShader("Geometry")->SetUniformEx("v_cameraMatrix", camera.ComputeProjectionMatrix() * 
        camera.ComputeViewMatrix());

    // Assign the material shader uniforms
    const Geometry::Material& material = geometry.GetMaterialData();

    ShaderSystem::GetInstance().GetShader("Geometry")->SetUniformEx("f_material.m_diffuseColor", material.m_diffuseColor);
    ShaderSystem::GetInstance().GetShader("Geometry")->SetUniform("f_material.m_enableTextures", material.m_enableTextures);

    if (material.m_diffuseTexture)
    {
        ShaderSystem::GetInstance().GetShader("Geometry")->SetUniform("f_material.m_diffuseTexture", 0);
        material.m_diffuseTexture->Bind(0); // Bind the diffuse texture
    }

    // Bind the geometry vao
    geometry.GetVertexArray().Bind();

    // Draw the geometry
    if (geometry.GetRenderFunction() == Geometry::RenderFunction::RENDER_ARRAYS)
        glDrawArrays(GL_TRIANGLES, 0, geometry.GetCount());
    else if (geometry.GetRenderFunction() == Geometry::RenderFunction::RENDER_ELEMENTS)
        glDrawElements(GL_TRIANGLES, geometry.GetCount(), GL_UNSIGNED_INT, nullptr);
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