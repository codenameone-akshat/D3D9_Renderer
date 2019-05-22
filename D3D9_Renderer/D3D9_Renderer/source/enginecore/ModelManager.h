#pragma once

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
        std::vector<float> m_batchVertexTable;
        std::vector<float> m_batchIndexTable;
	};
}
