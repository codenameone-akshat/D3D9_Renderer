#include <cassert>

#include "Mesh.h"

namespace renderer
{
	Mesh::Mesh()
		:m_numVertices(0),
		m_numNormals(0),
        m_numTexcoords(0),
		m_numIndices(0),
        m_numTris(0),
		m_vertices(),
		m_normals(),
        m_texcoords(),
        m_indices(),
        m_name(),
        m_materialIndex(0)
	{
	}

	Mesh::~Mesh()
	{
        m_vertices.clear();
        m_normals.clear();
        m_indices.clear();
        m_texcoords.clear();

        m_numVertices = m_numNormals = m_numTexcoords = m_numIndices = 0;
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

    void Mesh::AppendTexCoords(float x, float y) noexcept
    {
        assert(m_numTexcoords > 0);
        m_texcoords.emplace_back(x);
        m_texcoords.emplace_back(y);
    }

    void Mesh::AppendTangents(float x, float y, float z) noexcept
    {
        assert(m_numTangents > 0);
        m_tangents.emplace_back(x);
        m_tangents.emplace_back(y);
        m_tangents.emplace_back(z);
    }

    void Mesh::AppendBiTangents(float x, float y, float z) noexcept
    {
        assert(m_numBiTangents > 0);
        m_biTangents.emplace_back(x);
        m_biTangents.emplace_back(y);
        m_biTangents.emplace_back(z);
    }

	void Mesh::AppendIndices(const uint32_t vertexA, const uint32_t vertexB, const uint32_t vertexC) noexcept
	{
		assert(m_numIndices > 0);
		m_indices.emplace_back(vertexA);
		m_indices.emplace_back(vertexB);
		m_indices.emplace_back(vertexC);
	}

	void Mesh::SetNumVertices(int32_t nVertices)
	{
		m_numVertices = nVertices;
		m_vertices.reserve(nVertices * 3);
	}

	void Mesh::SetNumNormals(int32_t nNormals)
	{
		m_numNormals = nNormals;
		m_normals.reserve(nNormals * 3);
	}

    void Mesh::SetNumTexCoords(int32_t nTexCoords)
    {
        m_numTexcoords = nTexCoords;
        m_texcoords.reserve(nTexCoords * 2);
    }

    void Mesh::SetNumTangents(int32_t nTangents)
    {
        m_numTangents = nTangents;
        m_tangents.reserve(nTangents * 3);
    }

    void Mesh::SetNumBiTangents(int32_t nBiTangents)
    {
        m_numBiTangents = nBiTangents;
        m_biTangents.reserve(nBiTangents * 3);
    }

    void Mesh::SetNumIndices(int32_t nIndices)
	{
		m_numIndices = nIndices;
		m_indices.reserve(nIndices);
	}
}