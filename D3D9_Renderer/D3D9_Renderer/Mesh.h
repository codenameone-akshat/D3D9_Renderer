#pragma once
#include <assimp/scene.h>
#include <vector>

#include "BaseObject.h"

namespace renderer
{
	class Mesh :
		public BaseObject
	{
		using super = BaseObject;
	public:
		Mesh();
		~Mesh();

		void AppendVertices(float x, float y, float z) noexcept;
		void AppendNormals(float x, float y, float z) noexcept;
		void AppendIndices(const int vertexA, const int vertexB, const int vertexC) noexcept;

		inline int GetNumVertices() { return m_numVertices; }
		inline int GetNumNormals() { return m_numNormals; }
		inline int GetNumIndices() { return m_numIndices; }

		void SetNumVertices(int nVertices);
		void SetNumNormals(int nNormals);
		void SetNumIndices(int nIndices);

		[[nodiscard]] inline std::vector<float> GetVertices() const { return m_vertices; }
		[[nodiscard]] inline std::vector<float> GetNormals() const { return m_normals; }
		[[nodiscard]] inline std::vector<int> GetIndices() const { return m_indices; }

	private:
		int m_numVertices;
		int m_numNormals;
		int m_numIndices;

		std::vector<float> m_vertices;
		std::vector<float> m_normals;
		std::vector<int> m_indices;
	};
}