#include "Mesh.h"

Mesh::Mesh(GameContext* gameContext, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
	: Super(gameContext)
{
	m_Vertices = vertices;
	m_Indices = indices;
	m_Textures = textures;
	m_DrawListElement.Init();
	InitRenderInfo();
}

DrawListElement& Mesh::GetDrawListElement()
{
	return m_DrawListElement;
}

void Mesh::InitRenderInfo()
{
	m_DrawListElement.vertexBuffer = &m_Vertices[0];
	m_DrawListElement.VBsize_inByte = sizeof(Vertex) * m_Vertices.size();
	m_DrawListElement.indexBuffer = &m_Indices[0];
	m_DrawListElement.IBsize = m_Indices.size();
	m_DrawListElement.IBsize_inByte = sizeof(unsigned int) * m_Indices.size();

	m_DrawListElement.attributeSizes.push_back(3);
	m_DrawListElement.attributeSizes.push_back(3);
	m_DrawListElement.attributeSizes.push_back(2);
	m_DrawListElement.vertextInfoSize = 8;
	m_DrawListElement.drawingPrimitive = DrawingPrimitives::TRIANGLES;
	m_DrawListElement.drawingMode = DrawingMode::DRAW_ELEMENT;

	for (Texture &texture : m_Textures)
	{
		m_DrawListElement.textures.push_back(texture);
	}

	m_DrawListElement.Init();
}
