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
        //>hash_table<texturefileHash, Vertices/Indices>
        std::unordered_map<size_t, std::vector<float>> m_batchVertexTable;
        std::unordered_map<size_t, std::vector<float>> m_batchIndexTable;
	};
}
