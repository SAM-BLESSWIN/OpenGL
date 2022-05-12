#pragma once

#include <vector>
#include <GL/glew.h>
#include "Renderer.h"

struct VertexBufferElement
{
	unsigned int count;
	unsigned int type;        
	unsigned char normalized; //GL_BOOLEAN type

	static unsigned int GetSizeofType(unsigned int type)
	{
		switch (type)
		{
			case GL_FLOAT:			return 4;
			case GL_UNSIGNED_INT:	return 4;
			case GL_UNSIGNED_BYTE:	return 1;
		}
		ASSERT(false); //throws breakpoint when any other type is received
		return 0;
	}
};

class VertexBufferLayout
{
private:
	std::vector<VertexBufferElement> m_Elements;
	unsigned int m_stride;


public:
	VertexBufferLayout() :m_stride(0) {};

	template<typename T>
	void Push(unsigned int count)
	{
		//static_assert(false);
	}

	template<>
	void Push<float>(unsigned int count)
	{
		m_Elements.push_back({count, GL_FLOAT, GL_FALSE});
		m_stride += count * VertexBufferElement::GetSizeofType(GL_FLOAT);
	}

	template<>
	void Push<unsigned int>(unsigned int count)
	{
		m_Elements.push_back({ count, GL_UNSIGNED_INT, GL_FALSE });
		m_stride += count * VertexBufferElement::GetSizeofType(GL_UNSIGNED_INT);
	}

	template<>
	void Push<unsigned char>(unsigned int count)
	{
		m_Elements.push_back({ count, GL_UNSIGNED_BYTE, GL_TRUE });
		m_stride += count * VertexBufferElement::GetSizeofType(GL_UNSIGNED_BYTE);
	}

	inline std::vector<VertexBufferElement> GetElements() const& { return m_Elements; }

	inline unsigned int GetStride() const { return m_stride; }
};