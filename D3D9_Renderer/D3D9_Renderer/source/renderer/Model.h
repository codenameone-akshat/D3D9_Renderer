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

	class Model :
		public BaseObject
	{
		using super = BaseObject;
	public:
		Model();
		~Model();

		void LoadModelAndParseData(std::string filepath);

		//>Getters
		inline int GetNumMeshes() const { return m_numMeshes; }
        inline int GetTotalTriangles() const { return m_numTris; }
        inline int GetTotalVertices() const { return m_totalVertices; }
        inline int GetTotalNormals() const { return m_totalNormals; }
        inline int GetTotalTexCoord() const { return m_totalTexCoords; }
        inline int GetTotalIndices() const { return m_totalIndices; }
		
        inline std::vector<std::shared_ptr<Mesh>> GetMeshes() const { return m_meshes; }
        
	private:
		void ProcessModel();

		const Scene* m_scene;

		Importer m_importer;
		std::vector<std::shared_ptr<Mesh>> m_meshes;

		int m_numMeshes;
        int m_numTris;
		int m_totalVertices;
        int m_totalNormals;
        int m_totalTexCoords;
		int m_totalIndices;
	};
}