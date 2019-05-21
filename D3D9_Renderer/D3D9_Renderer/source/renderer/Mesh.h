#pragma once

#include <assimp/scene.h>
#include <vector>

#include "../enginecore/BaseObject.h"
#include "Material.h"

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
        void AppendTexCoords(float x, float y) noexcept;
		void AppendIndices(const uint32_t vertexA, const uint32_t vertexB, const uint32_t vertexC) noexcept;
		
        inline int GetNumVertices() { return m_numVertices; }
		inline int GetNumNormals() { return m_numNormals; }
        inline int GetNumTexCoords() { return m_numTexcoords; }
        inline int GetNumIndices() { return m_numIndices; }

		void SetNumVertices(int nVertices);
		void SetNumNormals(int nNormals);
        void SetNumTexCoords(int nTexCoords);
        void SetNumIndices(int nIndices);
        inline void SetName(std::string textureFile) noexcept { m_name = textureFile; }

		[[nodiscard]] inline std::vector<float> GetVertices() const { return m_vertices; }
        [[nodiscard]] inline std::vector<float> GetNormals() const { return m_normals; }
        [[nodiscard]] inline std::vector<float> GetTexCoords() const { return m_texcoords; }
		[[nodiscard]] inline std::vector<uint32_t> GetIndices() const { return m_indices; }
        [[nodiscard]] inline std::string GetName() const { return m_name; }

	private:
		int m_numVertices;
		int m_numNormals;
		int m_numIndices;
        int m_numTexcoords;

		std::vector<float> m_vertices;
        std::vector<float> m_normals;
        std::vector<float> m_texcoords;
		std::vector<uint32_t> m_indices;

        std::string m_name;
        Material m_material;
	};
}