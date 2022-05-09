#include "VertexBuffer.h"
#include "Renderer.h"


VertexBuffer::VertexBuffer(const void* data,unsigned int size)
{
    GLCALL(glGenBuffers(1, &m_RendererID)); //bufferid contains the id of that buffer
   //opengl keeps using the binded id untill someother id is binded or this is unbinded
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); //selecting that id and setting to interpret as a array 
    GLCALL(glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW)); //allocating and initializing data
}

VertexBuffer::~VertexBuffer()
{
    GLCALL(glDeleteBuffers(1,&m_RendererID));
}

void VertexBuffer::Bind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, m_RendererID)); //selecting that id and setting to interpret as a array 
}

void VertexBuffer::Unbind() const
{
    GLCALL(glBindBuffer(GL_ARRAY_BUFFER, 0)); 
}
