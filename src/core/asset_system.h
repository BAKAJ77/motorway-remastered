#ifndef ASSET_SYSTEM_H
#define ASSET_SYSTEM_H

#include <graphics/shader_program.h>
#include <graphics/texture_2d.h>

#include <unordered_map>
#include <string>
#include <memory>

using ShaderProgramPtr = std::shared_ptr<ShaderProgram>;
using Texture2DPtr = std::shared_ptr<Texture2D>;

class AssetSystem
{
private:
	std::unordered_map<std::string, ShaderProgramPtr> m_storedShaders;
	std::unordered_map<std::string, Texture2DPtr> m_storedTextures;

	AssetSystem() = default;
public:
	~AssetSystem() = default;

	// Loads shader from file and keeps a copy of it, which can be accessed using the GetShader() method.
	void LoadShader(std::string_view nameID, std::string_view vshFilePath, std::string_view fshFilePath);

	// Loads image from file and keeps copy of it as a texture, which can be accessed using the GetTexture() method.
	void LoadTexture(std::string_view nameID, std::string_view imageFilePath, bool flipOnLoad, bool srgb);

	// Removes the stored shader that is attached to the specified ID.
	void RemoveShader(std::string_view nameID);

	// Removes the stored texture that is attached to the specified ID.
	void RemoveTexture(std::string_view nameID);

	// Returns reference to the stored shader that is attached to the specified ID.
	// If no shader is found with the specified ID, then nullptr will be returned.
	ShaderProgramPtr GetShader(std::string_view nameID) const;

	// Returns reference to the stored texture that is attached to the specified ID.
	// If no texture is found with the specified ID, then nullptr will be returned.
	Texture2DPtr GetTexture(std::string_view nameID) const;

	// Returns singleton instance of the class.
	static AssetSystem& GetInstance();
};

#endif