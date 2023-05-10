#ifndef INDEX_BUFFER_H
#define INDEX_BUFFER_H

class IndexBuffer
{
private:
	unsigned int m_RendererID;
public:
	explicit IndexBuffer(void* data, int count);
	IndexBuffer(const IndexBuffer&) = delete;
	~IndexBuffer();
	void Bind() const;
	void Unbind() const;
};


#endif