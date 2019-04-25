#include "Model.h"

#include <assimp/postprocess.h>
#include <assimp/mesh.h>

namespace d3dgfx
{
	Model::Model()
		:m_importer(),
		m_scene(nullptr),
		m_numMeshes(0),
		m_totalVertices(0),
		m_totalNormals(0),
		m_totalIndices(0),
        m_numTris(0),
		m_meshes()
	{
	}

	Model::~Model()
	{
		delete m_scene;
		m_meshes.clear();
	}

	void Model::LoadModel(std::string filepath)
	{
		const auto flags = aiProcess_Triangulate | aiProcess_ConvertToLeftHanded | aiProcess_JoinIdenticalVertices | aiProcess_RemoveComponent | aiProcess_FlipUVs;

		m_scene = m_importer.ReadFile(filepath, flags);
		m_numMeshes = m_scene->mNumMeshes;
		
		ProcessModel();
	}

	void Model::ProcessModel()
	{
		m_meshes.reserve(m_numMeshes);
		auto const meshes = m_scene->mMeshes;

		int totalVertices = 0;
		int totalIndices = 0;

		for (auto itr = 0; itr < m_numMeshes; ++itr)
		{
			Mesh* mesh = new Mesh();
			
			totalVertices += m_scene->mMeshes[itr]->mNumVertices;
			totalIndices += m_scene->mMeshes[itr]->mNumFaces * 3;
            m_numTris += m_scene->mMeshes[itr]->mNumFaces;

			mesh->SetNumVertices(m_scene->mMeshes[itr]->mNumVertices);
			mesh->SetNumNormals(m_scene->mMeshes[itr]->mNumVertices); //number of normals = number of vertices
			mesh->SetNumIndices(m_scene->mMeshes[itr]->mNumFaces * 3);
			const auto numVert = m_scene->mMeshes[itr]->mNumVertices;

			[[maybe_unused]] unsigned int iter = 0;
			for (iter = 0; iter < numVert; ++iter)
			{
				float x = m_scene->mMeshes[itr]->mVertices[iter].x;
				float y = m_scene->mMeshes[itr]->mVertices[iter].y;
				float z = m_scene->mMeshes[itr]->mVertices[iter].z;
				mesh->AppendVertices(x, y, z);

				x = m_scene->mMeshes[itr]->mNormals[iter].x;
				y = m_scene->mMeshes[itr]->mNormals[iter].y;
				z = m_scene->mMeshes[itr]->mNormals[iter].z;
				mesh->AppendNormals(x, y, z);
			}

			const auto numFaces = m_scene->mMeshes[itr]->mNumFaces;
			for (iter = 0; iter < numFaces; ++iter)
			{
				const int vertexA = m_scene->mMeshes[itr]->mFaces[iter].mIndices[0];
				const int vertexB = m_scene->mMeshes[itr]->mFaces[iter].mIndices[1];
				const int vertexC = m_scene->mMeshes[itr]->mFaces[iter].mIndices[2];

				mesh->AppendIndices(vertexA, vertexB, vertexC);
			}

            m_totalVertices = totalVertices;
            m_totalNormals = totalVertices;
            m_totalIndices = totalIndices;

			m_meshes.emplace_back(mesh);
            delete mesh;
		}
	}
}