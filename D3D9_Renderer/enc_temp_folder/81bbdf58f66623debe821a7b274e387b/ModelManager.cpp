#include "ModelManager.h"

namespace renderer
{
	ModelManager::ModelManager()
		:m_model(new Model())
	{
	}
	ModelManager::~ModelManager()
	{
        delete m_model;
	}
	void ModelManager::AddModelToWorld(IDirect3DDevice9* deviceRef, std::string filePath)
	{
        m_deviceRef = deviceRef;
		/*std::unique_ptr<Model> model = std::make_unique<Model>();

		model->LoadModelAndParseData(filePath);*/
        LoadModel();
	}

	void ModelManager::LoadModel()
	{
		std::string filename = "data/Content/Sponza.fbx"; //get files to load from somewhere else

		m_model->LoadModelAndParseData(m_deviceRef, filename);

		int32_t vBufferVertexCount = 0;
		int32_t iBufferIndexCount = 0;
		int32_t primitiveCount = 0;

		std::vector<PositionVertex> positionVertices;
		std::vector<uint32_t> positionIndices;

        uint32_t indexOffset(0);
        uint32_t batchIBufferOffset(0);

		vBufferVertexCount = m_model->GetTotalVertices();
		iBufferIndexCount = m_model->GetTotalIndices();
		primitiveCount = m_model->GetTotalTriangles();

		auto meshList = m_model->GetMeshes();
		auto numMaterials = m_model->GetTotalMaterials();
		std::vector<BatchDesc> batchDescs(numMaterials);

		for (auto mesh : meshList)
		{
			auto meshVertices = mesh->GetVertices();
			auto meshNormals = mesh->GetNormals();
            auto meshTexCoords = mesh->GetTexCoords();
            for (auto vitr = meshVertices.begin(), nitr = meshNormals.begin(), titr = meshTexCoords.begin();
                vitr != meshVertices.end() && nitr != meshNormals.end() && titr != meshTexCoords.end();
                vitr += 3, nitr += 3, titr += 2)
            {
                positionVertices.push_back({ *vitr, *(vitr + 1), *(vitr + 2), //vertices
                    *nitr, *(nitr + 1), *(nitr + 2), //normals
                    *(titr), *(titr + 1) }); //texcoords
            }

			auto meshIndices = mesh->GetIndices();
			for (auto index : meshIndices)
				positionIndices.push_back(index + indexOffset);

            indexOffset += static_cast<uint32_t>(mesh->GetNumVertices());

            //update batch list for offsets
            auto matIndex = mesh->GetMaterialIndex();
            batchIBufferOffset += static_cast<uint32_t>(mesh->GetNumIndices()+1);
            if (matIndex < (numMaterials - 1))
            {
                batchDescs[matIndex + 1].vBufferOffsetCount += mesh->GetNumVertices();
                batchDescs[matIndex + 1].indexStart += batchIBufferOffset;
            }
            batchDescs[matIndex].primitiveCount += mesh->GetNumTris();
            batchDescs[matIndex].vertexCount += mesh->GetNumVertices();
		}

		m_batchDesc.insert(m_batchDesc.end(), batchDescs.begin(), batchDescs.end()); //useful when multiple models

		positionVertices.shrink_to_fit();
		positionIndices.shrink_to_fit();

        m_positionVertices = positionVertices;
        m_positionIndices = positionIndices;

		m_vBufferVertexCount = vBufferVertexCount;
		m_iBufferIndexCount = iBufferIndexCount;
		m_primitiveCount = primitiveCount;
	}
}