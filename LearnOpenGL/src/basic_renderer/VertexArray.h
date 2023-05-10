#ifndef VERTEX_ARRAY_H
#define VERTEX_ARRAY_H

#include "VertexBuffer.h"
#include "VertexLayout.h"


class VertexArray
{
private:
	unsigned int m_RendererID;
public:
	explicit VertexArray();
	VertexArray(const VertexArray&) = delete;
	~VertexArray();
	void Bind();
	void Unbind();
};


#endif