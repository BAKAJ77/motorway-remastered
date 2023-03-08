#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

#include <vector>

class VertexBuffer
{
public:
	struct Layout
	{
	public:
		uint32_t m_index, m_type, m_size, m_divisor;
		size_t m_strideBytes, m_offsetBytes;
		bool m_normalize;
	};
private:
	uint32_t m_id;
	std::vector<Layout> m_vertexLayouts;
public:
	VertexBuffer();
	VertexBuffer(const void* data, size_t size, uint32_t usage);
	VertexBuffer(const VertexBuffer& other) = delete;
	VertexBuffer(VertexBuffer&& temp) noexcept;

	~VertexBuffer();

	VertexBuffer& operator=(const VertexBuffer& other) = delete;
	VertexBuffer& operator=(VertexBuffer&& temp) noexcept;

	// Pushes vertex layout to vector which is used for configuring the vertex attributes.
	void PushLayout(uint32_t index, uint32_t type, uint32_t size, size_t strideBytes, size_t offsetBytes = 0, uint32_t divisor = 0,
		bool normalize = false);

	// Updates the data at the specified offset in the buffer with the new data provided.
	void ModifyData(const void* data, size_t offset, size_t size);

	// Binds the vertex buffer.
	void Bind() const;

	// Unbinds the vertex buffer.
	void Unbind() const;

	// Returns the vector containing all the specified vertex layouts.
	const std::vector<Layout>& GetVertexLayouts() const;

	// Returns the ID of the vertex buffer.
	uint32_t GetID() const;
};

#endif
