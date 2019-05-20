#include "ModelManager.h"

namespace renderer
{
	ModelManager::ModelManager()
	{
	}
	ModelManager::~ModelManager()
	{
	}
	void ModelManager::AddModelToWorld(std::string filePath)
	{
		std::unique_ptr<Model> model = std::make_unique<Model>();

		model->LoadModelAndParseData(filePath);

		//m_modelList.push_back(std::move(model));
	}
}