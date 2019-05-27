#pragma once

#include <string>
#include <assimp/Importer.hpp>
#include <vector>
#include <memory>
#include <cassert>

#include "../enginecore/BaseObject.h"
#include "Mesh.h"

namespace renderer
{
	using Scene = aiScene;
	using Importer = Assimp::Importer;

    struct MaterialDesc
    {
        uint32_t textureCount;
        std::vector<std::string> diffuseTexture;
        std::vector<std::string> normalTexture;
    };

	class Model :
		public BaseObject
	{
		using super = BaseObject;
	public:
		Model();
		~Model();

		void LoadModelAndParseData(IDirect3DDevice9* device, std::string filepath);

		//>Getters
		inline int32_t GetTotalMeshes() const { return m_numMeshes; }
		inline size_t GetTotalMaterials() const { return m_materials.size(); }
        inline int32_t GetTotalTriangles() const { return m_numTris; }
        inline int32_t GetTotalVertices() const { return m_totalVertices; }
        inline int32_t GetTotalNormals() const { return m_totalNormals; }
        inline int32_t GetTotalTexCoord() const { return m_totalTexCoords; }
        inline int32_t GetTotalIndices() const { return m_totalIndices; }
		
        inline std::vector<std::shared_ptr<Mesh>> GetMeshes() const { return m_meshes; }
        inline Material* GetMaterialAtIndex(uint32_t matIndex) const { assert(matIndex < m_materials.size()); return m_materials[matIndex]; }
        
	private:
		void ProcessModelVertexIndex();
        void ProcessModelMaterials(aiMaterial** materials, uint32_t materialCount);

        std::string m_fileDir;
        IDirect3DDevice9* m_deviceRef;

		const Scene* m_scene;

		Importer m_importer;
		
        std::vector<std::shared_ptr<Mesh>> m_meshes;
        std::vector<Material*> m_materials;

		int32_t m_numMeshes;
        int32_t m_numTris;
		int32_t m_totalVertices;
        int32_t m_totalNormals;
        int32_t m_totalTexCoords;
		int32_t m_totalIndices;
	};
}