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

	void AttachBuffers(const VertexBuffer& vbo, const IndexBuffer* ibo = nullptr);

	void Bind() const;
	void Unbind() const;

	const uint32_t& GetID() const;
};

#endif
