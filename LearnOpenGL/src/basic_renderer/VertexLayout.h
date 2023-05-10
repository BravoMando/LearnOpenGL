#ifndef VERTEX_LAYOUT_H
#define VERTEX_LAYOUT_H

#include "Render.h"

#include <vector>

struct VertexElement
{
	unsigned int type;
	unsigned int count;
	unsigned char normalized;
};

class VertexLayout
{
private:
	std::vector<VertexElement> m_Elements;
	unsigned int m_Stride;
public:
	explicit VertexLayout() :m_Stride(0) {}
	VertexLayout(const VertexLayout&) = delete;
	~VertexLayout()
	{
		if (!m_Elements.empty())
		{
			m_Elements.clear();
		}
	}
	template<typename T>
	void Push(unsigned int dimension)
	{
		static_assert(true);
	}
	template<>
	void Push<float>(unsigned int dimension)
	{
		m_Elements.push_back({ GL_FLOAT,dimension,GL_FALSE });
		m_Stride += dimension * sizeof(float);
	}
	template<>
	void Push<unsigned int>(unsigned int dimension)
	{
		m_Elements.push_back({ GL_UNSIGNED_INT,dimension,GL_TRUE });
		m_Stride += dimension * sizeof(unsigned int);
	}
	template<>
	void Push<unsigned char>(unsigned int dimension)
	{
		m_Elements.push_back({ GL_UNSIGNED_BYTE,dimension,GL_TRUE });
		m_Stride += dimension * sizeof(unsigned char);
	}

	inline unsigned int GetStride()const { return m_Stride; }
	inline const std::vector<VertexElement>& GetElements()const { return m_Elements; }
};


#endif