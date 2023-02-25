#ifndef TEXTURE_SYSTEM_H
#define TEXTURE_SYSTEM_H

#include <graphics/texture_2d.h>
#include <unordered_map>
#include <string>

class TextureSystem
{
private:
	std::unordered_map<std::string, Texture2D> m_storedTextures;
	
	TextureSystem() = default;
public:
	~TextureSystem() = default;

	// Loads image from file and keeps copy of it as a texture, which can be accessed using the GetTexture() method.
	void Load(std::string_view id, std::string_view filePath, bool flipOnLoad, bool srgb);

	// Destroys the stored texture that is attached to the specified ID.
	void Destroy(std::string_view id);

	// Returns reference to the stored texture that is attached to the specified ID.
	Texture2D& GetTexture(std::string_view id);

	// Returns singleton instance of the class.
	static TextureSystem& GetInstance();
};

#endif