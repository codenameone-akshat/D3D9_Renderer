#pragma once
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
		std::vector<std::unique_ptr<Model>> m_modelList; //entire model list to render in the world 
	};
}
