#include "ModelManager.h"

namespace renderer
{
	ModelManager::ModelManager()
		:m_model(make_unique<Model>())
	{
	}
	ModelManager::~ModelManager()
	{
	}
	void ModelManager::AddModelToWorld(std::string filePath)
	{
		/*std::unique_ptr<Model> model = std::make_unique<Model>();

		model->LoadModelAndParseData(filePath);*/
	}

//	void ModelManager::tempFunc()
//	{
//		std::string filename = "data/Content/Sponza.fbx"; //get files to load from somewhere else
//
//		m_model->LoadModelAndParseData(m_device->GetDeviceObject(), filename);
//
//		m_modelList.push_back(std::move(m_model));
//
//		int32_t vBufferVertexCount = 0;
//		int32_t iBufferIndexCount = 0;
//		int32_t primitiveCount = 0;
//
//		std::vector<PositionVertex> positionVertices;
//		std::vector<uint32_t> positionIndices;
//
//		uint32_t indexOffset(0);
//
//		vBufferVertexCount += m_model->GetTotalVertices();
//		iBufferIndexCount += m_model->GetTotalIndices();
//		primitiveCount += m_model->GetTotalTriangles();
//
//		auto meshList = m_model->GetMeshes();
//		auto numMaterials = Model->GetTotalMaterials();
//		std::vector<BatchDesc> batchDescs(numMaterials);
//
//		for (auto mesh : meshList)
//		{
//			auto meshVertices = mesh->GetVertices();
//			auto meshNormals = mesh->GetNormals();
//			for (auto vitr = meshVertices.begin(), nitr = meshNormals.begin();
//				vitr != meshVertices.end() && nitr != meshNormals.end();
//				vitr += 3, nitr += 3)
//			{
//				positionVertices.push_back({ *vitr, *(vitr + 1), *(vitr + 2), *nitr, *(nitr + 1), *(nitr + 2) });
//			}
//
//			auto meshIndices = mesh->GetIndices();
//			for (auto index : meshIndices)
//				positionIndices.push_back(index + indexOffset);
//
//			indexOffset += static_cast<uint32_t>(mesh->GetNumVertices());
//			
//			//update batch list for offsets
//			batchDescs[mesh->GetMaterialIndex()].vBufferOffsetBytes += mesh->GetNumVertices();
//		}
//
//		m_batchDesc.insert(m_batchDesc.end(), batchDescs.begin(), batchDescs.end()); //useful when multiple models
//
//		positionVertices.shrink_to_fit();
//		positionIndices.shrink_to_fit();
//
//		m_vBufferVertexCount = vBufferVertexCount;
//		m_iBufferIndexCount = iBufferIndexCount;
//		m_primitiveCount = primitiveCount;
//	}
}