#ifndef SHADER_PROGRAM_H
#define SHADER_PROGRAM_H

#include <string_view>
#include <unordered_map>
#include <glm/glm.hpp>

class ShaderProgram
{
private:
	enum class Operation { COMPILATION, LINKAGE };

	uint32_t m_id;
	mutable std::unordered_map<std::string, uint32_t> m_uniformLocationCache;

	// Checks if the compilation or linkage operation on the shader was successful.
	// Throws a formatted exception if any errors were thrown by OpenGL.
	void CheckShaderOperation(const uint32_t& id, Operation operation) const;

	// Returns the location unit of the specified uniform.
	uint32_t GetUniformLocation(std::string_view uniformName) const;
public:
	ShaderProgram();
	ShaderProgram(std::string_view vshFilePath, std::string_view fshFilePath);
	ShaderProgram(const ShaderProgram& other) = delete;
	ShaderProgram(ShaderProgram&& temp) noexcept;
	
	~ShaderProgram();

	ShaderProgram& operator=(const ShaderProgram& other) = delete;
	ShaderProgram& operator=(ShaderProgram&& temp) noexcept;

	// Assigns the given value to the specifed shader uniform.
	// This function (and overloads) are for simple types like integers, use SetUniformEx() for larger types like matrices.
	void SetUniform(std::string_view uniformName, int value) const;
	void SetUniform(std::string_view uniformName, float value) const;
	void SetUniform(std::string_view uniformName, bool value) const;

	// Assigns the given value to the specifed shader uniform.
	// This function (and overloads) are for larger types like matrices, use SetUniform() for simple types like integer.
	void SetUniformEx(std::string_view uniformName, const glm::vec2& vector) const;
	void SetUniformEx(std::string_view uniformName, const glm::vec3& vector) const;
	void SetUniformEx(std::string_view uniformName, const glm::vec4& vector) const;
	void SetUniformEx(std::string_view uniformName, const glm::mat3& matrix) const;
	void SetUniformEx(std::string_view uniformName, const glm::mat4& matrix) const;

	// Binds the shader program.
	void Bind() const;

	// Unbinds the shader program.
	void Unbind() const;

	// Returns the ID of the shader program.
	const uint32_t& GetID() const;
};

#endif
