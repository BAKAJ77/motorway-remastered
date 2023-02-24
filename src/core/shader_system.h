#ifndef SHADER_SYSTEM_H
#define SHADER_SYSTEM_H

#include <graphics/shader_program.h>
#include <unordered_map>
#include <memory>

class ShaderSystem
{
private:
	std::unordered_map<std::string, ShaderProgram> m_storedShaders;

	ShaderSystem() = default;
public:
	~ShaderSystem() = default;

	// Loads and keeps copy of the shader for which can be accessed using the GetShader() method.
	void Load(const std::string_view id, ShaderProgram&& shader);

	// Destroys the shader with the specified ID.
	void Destroy(const std::string_view id);

	// Returns reference to the shader with the specified ID.
	ShaderProgram& GetShader(const std::string_view& id);

	// Returns singleton instance of the class.
	static ShaderSystem& GetInstance();
};

#endif