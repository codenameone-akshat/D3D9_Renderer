#include <assimp/postprocess.h>
#include <assimp/mesh.h>
#include <cassert>

#include "Model.h"

namespace renderer
{
    Model::Model()
        :m_importer(),
        m_scene(nullptr),
        m_numMeshes(0),
        m_totalVertices(0),
        m_totalNormals(0),
        m_totalTexCoords(0),
        m_totalIndices(0),
        m_numTris(0),
        m_meshes()
    {
    }

    Model::~Model()
    {
        m_meshes.clear();
    }

    void Model::LoadModelAndParseData(std::string filepath)
    {
        const auto flags = aiProcess_CalcTangentSpace |
            aiProcess_Triangulate |
            aiProcess_ConvertToLeftHanded |
            aiProcess_JoinIdenticalVertices |
            aiProcess_RemoveRedundantMaterials | aiProcess_ValidateDataStructure |
            aiProcess_FlipUVs | aiProcess_OptimizeMeshes;

        m_scene = m_importer.ReadFile(filepath, flags);
        assert(m_scene != nullptr);
        m_numMeshes = m_scene->mNumMeshes;

        ProcessModel();
    }

    void Model::ProcessModel()
    {
        m_meshes.reserve(m_numMeshes);
        auto const meshes = m_scene->mMeshes;
        auto const materials = m_scene->mMaterials;
        auto const numMaterials = m_scene->mNumMaterials;

        int totalVertices = 0;
        int totalIndices = 0;

        for (uint32_t itr = 0; itr < numMaterials; ++itr)
        {
            aiString path;
            materials[itr]->GetTexture(aiTextureType_DIFFUSE, 0, &path);
            materials[itr]->GetTexture(aiTextureType_NORMALS, 0, &path);
        }

        for (auto itr = 0; itr < m_numMeshes; ++itr)
        {
            std::shared_ptr<Mesh> mesh = std::make_shared<Mesh>();

            totalVertices += meshes[itr]->mNumVertices;
            totalIndices += meshes[itr]->mNumFaces * 3;
            m_numTris += meshes[itr]->mNumFaces;

            mesh->SetNumVertices(meshes[itr]->mNumVertices);
            mesh->SetNumNormals(meshes[itr]->mNumVertices); //number of normals = number of vertices
            mesh->SetNumTexCoords(meshes[itr]->mNumVertices); //number of texcoord = number of vertices
            mesh->SetNumIndices(meshes[itr]->mNumFaces * 3);

            const auto numVert = meshes[itr]->mNumVertices;

            unsigned int iter = 0;
            for (iter = 0; iter < numVert; ++iter)
            {
                float x = meshes[itr]->mVertices[iter].x;
                float y = meshes[itr]->mVertices[iter].y;
                float z = meshes[itr]->mVertices[iter].z;
                mesh->AppendVertices(x, y, z);

                x = meshes[itr]->mNormals[iter].x;
                y = meshes[itr]->mNormals[iter].y;
                z = meshes[itr]->mNormals[iter].z;
                mesh->AppendNormals(x, y, z);

                x = meshes[itr]->mTextureCoords[0][iter].x;
                y = meshes[itr]->mTextureCoords[0][iter].y;
                mesh->AppendTexCoords(x, y);
            }

            const auto numFaces = meshes[itr]->mNumFaces;
            for (iter = 0; iter < numFaces; ++iter)
            {
                const uint32_t vertexA = meshes[itr]->mFaces[iter].mIndices[0];
                const uint32_t vertexB = meshes[itr]->mFaces[iter].mIndices[1];
                const uint32_t vertexC = meshes[itr]->mFaces[iter].mIndices[2];

                mesh->AppendIndices(vertexA, vertexB, vertexC);
            }
            mesh->SetName(meshes[itr]->mName.C_Str());
            auto id = meshes[itr]->mMaterialIndex;
            m_meshes.emplace_back(std::move(mesh));
        }

        m_totalVertices = totalVertices;
        m_totalNormals = totalVertices;
        m_totalIndices = totalIndices;
    }
}