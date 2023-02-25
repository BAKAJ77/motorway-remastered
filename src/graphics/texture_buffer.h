#ifndef TEXTURE_BUFFER_H
#define TEXTURE_BUFFER_H

#include <glm/glm.hpp>

typedef unsigned int uint32_t;

class TextureBuffer // This is purely an abstract class, it shouldn't be used directly (as in creating objects of it)
{
protected:
	uint32_t m_id, m_target;
	glm::ivec2 m_size;
public:
	TextureBuffer();
	TextureBuffer(uint32_t target, const glm::ivec2& size);
	TextureBuffer(const TextureBuffer& other) = delete;

	virtual ~TextureBuffer();

	TextureBuffer& operator=(const TextureBuffer& other) = delete;

	// Selects which magnification and minification filter method to apply on the texture. 
	void SetFilter(uint32_t min, uint32_t mag) const;

	// Selects the wrapping methods to apply on the S axis (x) and T axis (y) of the texture.
	void SetWrap(uint32_t sAxis, uint32_t tAxis) const;
	
	// Binds the texture buffer.
	void Bind() const;

	// Binds the texture buffer and activates the specified texture unit.
	void Bind(int textureUnit) const;

	// Unbinds the texture buffer.
	void Unbind() const;

	// Returns the ID of the texture buffer.
	const uint32_t& GetID() const;

	// Returns the target enum of the texture buffer.
	const uint32_t& GetTarget() const;

	// Returns the size of the texture buffer.
	const glm::ivec2& GetSize() const;
};

#endif