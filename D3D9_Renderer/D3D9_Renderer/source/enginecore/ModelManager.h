#pragma once

#include<vector>

#include "../renderer/Model.h"
#include "../renderer/d3d9/VertexDefs.h"
#include "Batch.h"

namespace renderer
{
	class ModelManager
	{
	public:
		ModelManager();
		~ModelManager();

		void AddModelToWorld(IDirect3DDevice9* deviceRef, std::string filePath);
		void LoadModel();

        inline Model* GetModel() const { return m_model; }
        inline std::vector<PositionVertex> GetVertexBufferData() { return m_positionVertices; }
        inline std::vector<uint32_t> GetIndexBufferData() { return m_positionIndices; }

        inline int32_t GetVBufferCount() { return m_vBufferVertexCount; }
        inline int32_t GetIBufferCount() { return m_iBufferIndexCount; }
        inline int32_t GetPrimitiveCount() { return m_primitiveCount; }
        inline std::vector<BatchDesc> GetBatchList() { return m_batchDesc; }
	private:
        
        IDirect3DDevice9* m_deviceRef;
        
        Model* m_model;
		std::vector<BatchDesc> m_batchDesc;
        std::vector<PositionVertex> m_positionVertices;
        std::vector<uint32_t> m_positionIndices;

        int32_t m_vBufferVertexCount;
        int32_t m_iBufferIndexCount;
        int32_t m_primitiveCount;
	};
}
