#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include <graphics/vertex_buffer.h>
#include <graphics/index_buffer.h>

class VertexArray
{
private:
	uint32_t m_id;
public:
	VertexArray();
	~VertexArray();

	// Attaches the given vertex buffer (and index buffer if given) to the vertex array object.
	void AttachBuffers(const VertexBuffer& vbo, const IndexBuffer* ibo = nullptr);

	// Binds the vertex array object.
	void Bind() const;

	// Unbinds the vertex array object.
	void Unbind() const;

	// Returns the ID of the vertex array object.
	const uint32_t& GetID() const;
};

#endif
