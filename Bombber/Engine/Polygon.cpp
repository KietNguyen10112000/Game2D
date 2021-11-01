#include "Polygon.h"

#include <SDL.h>
#include <GL/glew.h>
#include <SDL_opengl.h>
#include <algorithm>

Polygon::Polygon(size_t capacity)
{
	capacity = std::min(capacity, (size_t)MAX_VERTEX);

	m_vertices.reserve(capacity);
	m_indices.reserve(capacity * 3  + 2);

	m_vertices.resize(1);
	m_indices.resize(1);

	glGenBuffers(1, &m_vb);
	glBindBuffer(GL_ARRAY_BUFFER, m_vb);
	glBufferData(GL_ARRAY_BUFFER, capacity * sizeof(Vertex), &m_vertices[0], GL_DYNAMIC_DRAW);

	glGenBuffers(1, &m_ib);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (capacity * 3 + 2) * sizeof(uint32_t), &m_indices[0], GL_DYNAMIC_DRAW);


	m_vertices.clear();
	m_indices.clear();
}

Polygon::~Polygon()
{
	glDeleteBuffers(1, &m_vb);
	glDeleteBuffers(1, &m_ib);
}

void Polygon::Update()
{
	if (!m_needUpdate) return;

    glBindBuffer(GL_ARRAY_BUFFER, m_vb);
    void* ptr = glMapBuffer(GL_ARRAY_BUFFER, GL_WRITE_ONLY);
    memcpy(ptr, &m_vertices[0], m_vertices.size() * sizeof(Vertex));
    glUnmapBuffer(GL_ARRAY_BUFFER);

	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ib);
	ptr = glMapBuffer(GL_ELEMENT_ARRAY_BUFFER, GL_WRITE_ONLY);
	memcpy(ptr, &m_indices[0], m_indices.size() * sizeof(uint32_t));
	glUnmapBuffer(GL_ELEMENT_ARRAY_BUFFER);

	m_needUpdate = false;
}
