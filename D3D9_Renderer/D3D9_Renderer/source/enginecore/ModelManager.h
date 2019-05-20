#pragma once
#include<unordered_map>
#include<functional>
#include<vector>

#include "../renderer/Model.h"

namespace renderer
{
	class ModelManager
	{
	public:
		ModelManager();
		~ModelManager();

		void AddModelToWorld(std::string filePath);

	private:
		//std::vector<std::unique_ptr<Model>> m_modelList; //entire model list to render in the world

        //>hash_table<texturefileHash, Vertices/Indices>
        std::unordered_map<size_t, std::vector<float>> m_batchVertexTable;
        std::unordered_map<size_t, std::vector<float>> m_batchIndexTable;
	};
}
