#pragma once

#include <string>
#include <assimp/Importer.hpp>
#include <vector>

#include "BaseObject.h"
#include "Mesh.h"

namespace d3dgfx
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

		void LoadModel(std::string filepath);

		//>Getters
		inline int GetNumMeshes() const { return m_numMeshes; }
		inline std::vector<Mesh> GetMeshes() const { return m_meshes; }
		inline Mesh GetMesheAtIndex(const unsigned int index) const { return m_meshes[index]; }

	private:
		void ProcessModel();

		const Scene* m_scene;

		Importer m_importer;
		std::vector<Mesh> m_meshes;

		int m_numMeshes;
		int m_totalVertices;
		int m_totalNormals;
		int m_totalIndices;
	};
}