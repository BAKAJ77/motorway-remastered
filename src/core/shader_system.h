#ifndef SHADER_SYSTEM_H
#define SHADER_SYSTEM_H

#include <graphics/shader_program.h>
#include <unordered_map>
#include <memory>

using ShaderProgramPtr = std::shared_ptr<ShaderProgram>;

class ShaderSystem
{
private:
	std::unordered_map<std::string, ShaderProgramPtr> m_storedShaders;

	ShaderSystem() = default;
public:
	~ShaderSystem() = default;

	// Loads shader from file and keeps a copy of it, which can be accessed using the GetShader() method.
	void Load(std::string_view id, ShaderProgram&& shader);

	// Removes the stored shader that is attached to the specified ID.
	void Remove(std::string_view id);

	// Returns reference to the stored shader that is attached to the specified ID.
	ShaderProgramPtr GetShader(std::string_view id);

	// Returns singleton instance of the class.
	static ShaderSystem& GetInstance();
};

#endif