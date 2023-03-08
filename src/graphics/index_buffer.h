#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

typedef unsigned int uint32_t;

class IndexBuffer
{
private:
	uint32_t m_id;
public:
	IndexBuffer();
	IndexBuffer(const void* data, size_t size, uint32_t usage);
	IndexBuffer(const IndexBuffer& other) = delete;
	IndexBuffer(IndexBuffer&& temp) noexcept;

	~IndexBuffer();

	IndexBuffer& operator=(const IndexBuffer& other) = delete;
	IndexBuffer& operator=(IndexBuffer&& temp) noexcept;

	// Updates the data at the specified offset in the buffer with the new data provided.
	void ModifyData(const void* data, size_t offset, size_t size);

	// Binds the index buffer.
	void Bind() const;

	// Unbinds the index buffer.
	void Unbind() const;

	// Returns the ID of the index buffer.
	uint32_t GetID() const;
};

#endif
