#include <cassert>

#include "Mesh.h"

namespace renderer
{
	Mesh::Mesh()
		:m_numVertices(0),
		m_numNormals(0),
		m_numIndices(0),
		m_vertices(),
		m_normals(),
		m_indices()
	{
	}

	Mesh::~Mesh()
	{
        m_vertices.clear();
        m_normals.clear();
        m_indices.clear();
	}

	void Mesh::AppendVertices(float x, float y, float z) noexcept
	{
		assert(m_numVertices > 0);
		m_vertices.emplace_back(x);
		m_vertices.emplace_back(y);
		m_vertices.emplace_back(z);
	}

	void Mesh::AppendNormals(float x, float y, float z) noexcept
	{
		assert(m_numNormals > 0);
		m_normals.emplace_back(x);
		m_normals.emplace_back(y);
		m_normals.emplace_back(z);
	}

	void Mesh::AppendIndices(const uint32_t vertexA, const uint32_t vertexB, const uint32_t vertexC) noexcept
	{
		assert(m_numIndices > 0);
		m_indices.emplace_back(vertexA);
		m_indices.emplace_back(vertexB);
		m_indices.emplace_back(vertexC);
	}

	void Mesh::SetNumVertices(int nVertices)
	{
		m_numVertices = nVertices;
		m_vertices.reserve(nVertices * 3);
	}

	void Mesh::SetNumNormals(int nNormals)
	{
		m_numNormals = nNormals;
		m_normals.reserve(nNormals * 3);
	}

	void Mesh::SetNumIndices(int nIndices)
	{
		m_numIndices = nIndices;
		m_indices.reserve(nIndices);
	}
}