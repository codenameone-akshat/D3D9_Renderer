#pragma once

#include<vector>

#include "../renderer/Model.h"

namespace renderer
{

	struct BatchDesc
	{
		BatchDesc()
			:vBufferOffsetBytes(0)
		{}

		size_t vBufferOffsetBytes;
	};

	class ModelManager
	{
	public:
		ModelManager();
		~ModelManager();

		void AddModelToWorld(std::string filePath);
		void ModelManager::tempFunc();
	private:
		std::unique_ptr<Model> m_model;
		std::vector<BatchDesc> m_batchDesc;
	};
}
