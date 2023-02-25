#ifndef TEXTURE_2D_H
#define TEXTURE_2D_H

#include <graphics/texture_buffer.h>

class Texture2D : public TextureBuffer
{
public:
	Texture2D() = default;
	Texture2D(const void* pixels, const glm::ivec2& size, uint32_t pixelDataType, uint32_t internalFormat, uint32_t format);
	Texture2D(Texture2D&& temp) noexcept;

	~Texture2D() = default;

	Texture2D& operator=(Texture2D&& temp) noexcept;

	// Updates the data at the specified offset in the buffer with the new pixel data provided.
	void ModifyData(const void* pixels, const glm::ivec2& offset, const glm::ivec2& size, uint32_t pixelDataType, uint32_t format);
};

#endif
