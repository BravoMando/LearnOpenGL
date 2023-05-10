#ifndef VERTEX_BUFFER_H
#define VERTEX_BUFFER_H

class VertexBuffer
{
private:
	unsigned int m_RendererID;
public:
	explicit VertexBuffer(void* data, int size);
	VertexBuffer(const VertexBuffer&) = delete;
	~VertexBuffer();
	void Bind() const;
	void Unbind() const;
};


#endif