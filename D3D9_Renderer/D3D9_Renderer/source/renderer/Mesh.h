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
        void AppendTangents(float x, float y, float z) noexcept;
        void AppendBiTangents(float x, float y, float z) noexcept;
        void AppendIndices(const uint32_t vertexA, const uint32_t vertexB, const uint32_t vertexC) noexcept;
		
		inline uint16_t GetMaterialIndex() const { return m_materialIndex; }
		inline int32_t GetNumVertices() const { return m_numVertices; }
		inline int32_t GetNumNormals() const { return m_numNormals; }
        inline int32_t GetNumTexCoords() const { return m_numTexcoords; }
        inline int32_t GetNumTangents() const { return m_numTangents; }
        inline int32_t GetNumBiTangents() const { return m_numBiTangents; }
        inline int32_t GetNumIndices() const { return m_numIndices; }
        inline int32_t GetNumTris() const { return m_numTris; }

		void SetNumVertices(int32_t nVertices);
		void SetNumNormals(int32_t nNormals);
        void SetNumTexCoords(int32_t nTexCoords);
        void SetNumTangents(int32_t nTangents);
        void SetNumBiTangents(int32_t nBiTangents);
        void SetNumIndices(int32_t nIndices);

        inline void SetNumTris(int32_t nTris) { m_numTris = nTris; }
        inline void SetMaterialIndex(int16_t index) { m_materialIndex = index; }
        inline void SetName(std::string textureFile) noexcept { m_name = textureFile; }

		[[nodiscard]] inline std::vector<float> GetVertices() const { return m_vertices; }
        [[nodiscard]] inline std::vector<float> GetNormals() const { return m_normals; }
        [[nodiscard]] inline std::vector<float> GetTexCoords() const { return m_texcoords; }
        [[nodiscard]] inline std::vector<float> GetTangents() const { return m_tangents; }
        [[nodiscard]] inline std::vector<float> GetBiTangents() const { return m_biTangents; }
		[[nodiscard]] inline std::vector<uint32_t> GetIndices() const { return m_indices; }
        [[nodiscard]] inline std::string GetName() const { return m_name; }

	private:
		uint16_t m_materialIndex;
		int32_t m_numVertices;
		int32_t m_numNormals;
        int32_t m_numTangents;
        int32_t m_numBiTangents;
		int32_t m_numIndices;
        int32_t m_numTexcoords;
        int32_t m_numTris;

		std::vector<float> m_vertices;
        std::vector<float> m_normals;
        std::vector<float> m_texcoords;
        std::vector<float> m_tangents;
        std::vector<float> m_biTangents;
		std::vector<uint32_t> m_indices;
        
        std::string m_name;
	};
}