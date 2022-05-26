#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "Renderer.h"

VertexArray::VertexArray()
{
    GLCALL(glGenVertexArrays(1, &m_RendererID));
}

VertexArray::~VertexArray()
{
    GLCALL(glDeleteVertexArrays(1,&m_RendererID));
}

void VertexArray::Bind() const
{
    GLCALL(glBindVertexArray(m_RendererID));
}

void VertexArray::Unbind() const
{
    GLCALL(glBindVertexArray(0));
}



void VertexArray::AddBuffer(const VertexBuffer& vbo, const VertexBufferLayout& layout)
{
    Bind(); //Bind VAO
	vbo.Bind(); //Bind Buffer

    /*Setting Layout*/
    const auto& elements = layout.GetElements();
    unsigned int offset = 0;

    for (unsigned int i=0;i<elements.size();i++)
    {
        const auto& element = elements[i];

        GLCALL(glEnableVertexAttribArray(i)); //enabling vertex atribute by passing its index

         /*Vertex Attributes*/
        //index - index of the attribute
        //size - count of data in each vertex
        //type - datatype of the attribute
        //normalize? - whether we want gpu to normalize data or not 
        //stride - amount of bytes between each vertex 
        //pointer - offset of each attribute in bytes
        GLCALL(glVertexAttribPointer(i, element.count, element.type, element.normalized, 
            layout.GetStride(), (const void*)offset)); //attribute

        offset += element.count * VertexBufferElement::GetSizeofType(element.type);
    }


}
