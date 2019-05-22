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
		
		inline uint16_t GetMaterialIndex() { return m_materialIndex; }
		inline int32_t GetNumVertices() { return m_numVertices; }
		inline int32_t GetNumNormals() { return m_numNormals; }
        inline int32_t GetNumTexCoords() { return m_numTexcoords; }
		inline int32_t GetNumIndices() { return m_numIndices; }

		void SetNumVertices(int32_t nVertices);
		void SetNumNormals(int32_t nNormals);
        void SetNumTexCoords(int32_t nTexCoords);
        void SetNumIndices(int32_t nIndices);
        void SetMaterialIndex(int16_t index);
        inline void SetName(std::string textureFile) noexcept { m_name = textureFile; }

		[[nodiscard]] inline std::vector<float> GetVertices() const { return m_vertices; }
        [[nodiscard]] inline std::vector<float> GetNormals() const { return m_normals; }
        [[nodiscard]] inline std::vector<float> GetTexCoords() const { return m_texcoords; }
		[[nodiscard]] inline std::vector<uint32_t> GetIndices() const { return m_indices; }
        [[nodiscard]] inline std::string GetName() const { return m_name; }

	private:
		uint16_t m_materialIndex;
		int32_t m_numVertices;
		int32_t m_numNormals;
		int32_t m_numIndices;
        int32_t m_numTexcoords;

		std::vector<float> m_vertices;
        std::vector<float> m_normals;
        std::vector<float> m_texcoords;
		std::vector<uint32_t> m_indices;
        
        std::string m_name;
	};
}