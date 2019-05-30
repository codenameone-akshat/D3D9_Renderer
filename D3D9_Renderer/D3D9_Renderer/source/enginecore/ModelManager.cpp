#include "ModelManager.h"
#include "../utils/Logger.h"

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
		m_model->LoadModelAndParseData(deviceRef, filePath);
		LoadModel();
	}

	void ModelManager::LoadModel()
	{
		int32_t vBufferVertexCount = 0;
		int32_t iBufferIndexCount = 0;
		int32_t primitiveCount = 0;

		std::vector<PositionVertex> positionVertices;
		std::vector<uint32_t> positionIndices;

        uint32_t indexOffset(0);

		vBufferVertexCount = m_model->GetTotalVertices();
		iBufferIndexCount = m_model->GetTotalIndices();
		primitiveCount = m_model->GetTotalTriangles();

		auto meshList = m_model->GetMeshes();
		auto numMaterials = m_model->GetTotalMaterials();
		std::vector<BatchDesc> batchDescs(numMaterials);

		for (auto mesh : meshList)
		{
			auto const meshVertices = mesh->GetVertices();
			auto const meshNormals = mesh->GetNormals();
            auto const meshTexCoords = mesh->GetTexCoords();

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
            
			if (matIndex < (numMaterials - 1))
            {
                batchDescs[matIndex + 1].indexStart += static_cast<uint32_t>(mesh->GetNumIndices());
            }

            batchDescs[matIndex].primitiveCount += mesh->GetNumTris();
            batchDescs[matIndex].vertexCount += mesh->GetNumVertices();
		}

        for (uint16_t itr = 0; itr < batchDescs.size(); ++itr)
        {
            if (itr > 0)
            {
                batchDescs[itr].indexStart += batchDescs[itr - 1].indexStart;
            }
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

	void ModelManager::SetShaderInputsForMaterialIndex(uint32_t index, ID3DXEffect* shader)
	{
		auto material = m_model->GetMaterialAtIndex(index);
		auto diffuseTex = material->GetTextureOfType(Material::TextureType::Diffuse);
		auto normalTex = material->GetTextureOfType(Material::TextureType::Normal);
		auto specTex = material->GetTextureOfType(Material::TextureType::Specular);
		auto opacityTex = material->GetTextureOfType(Material::TextureType::Opacity);

		shader->SetTexture("g_DiffuseTex", diffuseTex);
		shader->SetTexture("g_NormalTex", normalTex);
		shader->SetTexture("g_SpecularTex", specTex);
		shader->SetTexture("g_OpacityTex", opacityTex);
		shader->SetVector("g_dirLightDir", &D3DXVECTOR4(1.0f, 1.0f, 0.3f, 1.0f));
		shader->SetVector("g_dirLightColor", &D3DXVECTOR4(1.0f, 0.69f, 0.32f, 1.0f));
		shader->SetVector("g_ambientLight", &D3DXVECTOR4(0.4f, 0.8f, 0.99f, 1.0f));
		shader->SetFloat("g_ambientLightIntensity", 0.3f);
		shader->SetVector("g_specularLightColor", &D3DXVECTOR4(0.5f, 0.5f, 0.5f, 1.0f));
		shader->SetFloat("g_specIntensity", 180.0f);
	}
}