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

	void ModifyData(const void* data, size_t offset, size_t size);

	void Bind() const;
	void Unbind() const;

	const uint32_t& GetID() const;
};

#endif
