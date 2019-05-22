#pragma once

#include <string>
#include <assimp/Importer.hpp>
#include <vector>
#include <memory>

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
		inline int GetNumMeshes() const { return m_numMeshes; }
        inline int GetTotalTriangles() const { return m_numTris; }
        inline int GetTotalVertices() const { return m_totalVertices; }
        inline int GetTotalNormals() const { return m_totalNormals; }
        inline int GetTotalTexCoord() const { return m_totalTexCoords; }
        inline int GetTotalIndices() const { return m_totalIndices; }
		
        inline std::vector<std::shared_ptr<Mesh>> GetMeshes() const { return m_meshes; }
        
	private:
		void ProcessModelVertexIndex();
        void ProcessModelMaterials(aiMaterial** materials, uint32_t materialCount);

        std::string m_fileDir;
        IDirect3DDevice9* m_deviceRef;

		const Scene* m_scene;

		Importer m_importer;
		
        std::vector<std::shared_ptr<Mesh>> m_meshes;
        std::vector<Material> m_materials;

		int m_numMeshes;
        int m_numTris;
		int m_totalVertices;
        int m_totalNormals;
        int m_totalTexCoords;
		int m_totalIndices;
	};
}