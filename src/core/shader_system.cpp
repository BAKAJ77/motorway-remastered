#include <core/shader_system.h>
#include <util/logging_system.h>
#include <util/formatted_exception.h>

void ShaderSystem::Load(std::string_view id, ShaderProgram&& shader)
{
    if (m_storedShaders.find(id.data()) == m_storedShaders.end()) // Make sure the ID given isn't already taken
        m_storedShaders[id.data()] = std::make_shared<ShaderProgram>(std::move(shader));
    else
    {
        LoggingSystem::GetInstance().Output("Skipped shader load operation, the ID \"%s\" has already been used.",
            LoggingSystem::Severity::WARNING, id.data());
    }
}

void ShaderSystem::Remove(std::string_view id)
{
    m_storedShaders.erase(id.data());
}

ShaderProgramPtr ShaderSystem::GetShader(std::string_view id)
{
    auto shaderIterator = m_storedShaders.find(id.data());
    if (shaderIterator == m_storedShaders.end())
        throw FormattedException("No shader exists with the ID \"%s\".", id.data());

    return shaderIterator->second;
}

ShaderSystem& ShaderSystem::GetInstance()
{
    static ShaderSystem instance;
    return instance;
}
