#include <graphics/shader_program.h>
#include <util/formatted_exception.h>

#include <glad/glad.h>
#include <sstream>
#include <fstream>
#include <memory>

ShaderProgram::ShaderProgram() :
    m_id(0)
{}

ShaderProgram::ShaderProgram(std::string_view vshFilePath, std::string_view fshFilePath)
{
    // Firstly open the shader files
    std::ifstream vshFileStream(vshFilePath.data()), fshFileStream(fshFilePath.data());

    if (vshFileStream.fail())
        throw FormattedException("Failed to open the vertex shader file at path: %s", vshFilePath.data());

    if (fshFileStream.fail())
        throw FormattedException("Failed to open the fragment shader file at path: %s", fshFilePath.data());

    // Stream the contents from the files into their respective string streams
    std::stringstream vshStream, fshStream;
    vshStream << vshFileStream.rdbuf();
    fshStream << fshFileStream.rdbuf();

    const std::string vshContents = vshStream.str(), fshContents = fshStream.str();
    
    vshFileStream.close(); // We can now close the shader files
    fshFileStream.close();

    // Compile the streamed shader file contents
    const char* vshContentsPtr = vshContents.c_str(), *fshContentsPtr = fshContents.c_str();

    const uint32_t vshID = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshID, 1, &vshContentsPtr, nullptr);
    glCompileShader(vshID);

    this->CheckShaderOperation(vshID, Operation::COMPILATION);

    const uint32_t fshID = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshID, 1, &fshContentsPtr, nullptr);
    glCompileShader(fshID);

    this->CheckShaderOperation(fshID, Operation::COMPILATION);

    // Attach the compiled shaders to the shader program and link them
    m_id = glCreateProgram();
    glAttachShader(m_id, vshID);
    glAttachShader(m_id, fshID);
    glLinkProgram(m_id);

    this->CheckShaderOperation(m_id, Operation::LINKAGE);

    glDeleteShader(vshID); // We can delete the shader objects now
    glDeleteShader(fshID);
}

ShaderProgram::ShaderProgram(ShaderProgram&& temp) noexcept :
    m_id(temp.m_id)
{
    temp.m_id = 0;
}

ShaderProgram::~ShaderProgram()
{
    if (m_id != 0)
        glDeleteProgram(m_id);
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& temp) noexcept
{
    m_id = temp.m_id;
    temp.m_id = 0;

    return *this;
}

void ShaderProgram::CheckShaderOperation(const uint32_t& id, Operation operation) const
{
    std::unique_ptr<char> logBuffer = nullptr;
    int logLength = 0;

    switch (operation)
    {
    case Operation::COMPILATION:
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0)
        {
            logBuffer = std::unique_ptr<char>(new char[logLength]);
            glGetShaderInfoLog(id, logLength, nullptr, logBuffer.get());
        }
        break;
    case Operation::LINKAGE:
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &logLength);
        if (logLength > 0)
        {
            logBuffer = std::unique_ptr<char>(new char[logLength]);
            glGetProgramInfoLog(id, logLength, nullptr, logBuffer.get());
        }
        break;
    }

    if (logBuffer)
        throw FormattedException(logBuffer.get());
}

uint32_t ShaderProgram::GetUniformLocation(std::string_view uniformName) const
{
    // Look for the uniform in the cache map
    auto cacheMapIterator = m_uniformLocationCache.find(uniformName.data());
    if (cacheMapIterator != m_uniformLocationCache.end())
        return cacheMapIterator->second;

    // It's not in the cache map so add it into there
    const uint32_t uniformLocation = glGetUniformLocation(m_id, uniformName.data());
    m_uniformLocationCache[uniformName.data()] = uniformLocation;

    return uniformLocation;
}

void ShaderProgram::SetUniform(std::string_view uniformName, int value) const
{
    glUniform1i(this->GetUniformLocation(uniformName), value);
}

void ShaderProgram::SetUniform(std::string_view uniformName, float value) const
{
    glUniform1f(this->GetUniformLocation(uniformName), value);
}

void ShaderProgram::SetUniform(std::string_view uniformName, bool value) const
{
    glUniform1i(this->GetUniformLocation(uniformName), (int)value);
}

void ShaderProgram::SetUniformEx(std::string_view uniformName, const glm::vec2& vector) const
{
    glUniform2fv(this->GetUniformLocation(uniformName), 1, &vector[0]);
}

void ShaderProgram::SetUniformEx(std::string_view uniformName, const glm::vec3& vector) const
{
    glUniform3fv(this->GetUniformLocation(uniformName), 1, &vector[0]);
}

void ShaderProgram::SetUniformEx(std::string_view uniformName, const glm::vec4& vector) const
{
    glUniform4fv(this->GetUniformLocation(uniformName), 1, &vector[0]);
}

void ShaderProgram::SetUniformEx(std::string_view uniformName, const glm::mat3& matrix) const
{
    glUniformMatrix3fv(this->GetUniformLocation(uniformName), 1, false, &matrix[0][0]);
}

void ShaderProgram::SetUniformEx(std::string_view uniformName, const glm::mat4& matrix) const
{
    glUniformMatrix4fv(this->GetUniformLocation(uniformName), 1, false, &matrix[0][0]);
}

void ShaderProgram::Bind() const
{
    glUseProgram(m_id);
}

void ShaderProgram::Unbind() const
{
    glUseProgram(0);
}

const uint32_t& ShaderProgram::GetID() const
{
    return m_id;
}
