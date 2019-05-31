#include "Shader.h"
#include "../../utils/ComHelpers.h"
#include "../../utils/Logger.h"

namespace renderer
{
    Shader::Shader():
        m_shader(nullptr),
        m_shaderPath(),
        m_shaderDesc(),
        m_techniques(),
        m_deviceRef(nullptr)
    {
    }

    Shader::~Shader()
    {
        ComSafeRelease(m_shader);
        m_techniques.clear();
    }

    void Shader::CreateShader(IDirect3DDevice9* device, std::string shaderPath)
    {
        m_shaderPath = shaderPath;
        m_deviceRef = device;

        ID3DXBuffer* errorBuffer = nullptr;
        ComResult(D3DXCreateEffectFromFileA(m_deviceRef,
            m_shaderPath.c_str(), nullptr, nullptr,
            D3DXSHADER_DEBUG, nullptr, &m_shader, &errorBuffer));

        if (errorBuffer)
        {
            ::MessageBoxA(0, (char*)errorBuffer->GetBufferPointer(), 0, 0);
            Logger::GetInstance().LogInfo((char*)errorBuffer->GetBufferPointer());
        }
        ComSafeRelease(errorBuffer);

        m_shader->GetDesc(&m_shaderDesc);
        for (UINT32 itr = 0; itr < m_shaderDesc.Techniques; ++itr)
        {
            D3DXTECHNIQUE_DESC techDesc;
            D3DXHANDLE hTechnique = m_shader->GetTechnique(itr);
            m_shader->GetTechniqueDesc(hTechnique, &techDesc);
            m_techniques.emplace(std::make_pair(hTechnique, techDesc));
        }
    }

    void Shader::ReloadShader()
    {
        CreateShader(m_deviceRef, m_shaderPath);
    }

	void Shader::SetTechniqueAndBegin(D3DXHANDLE technique)
	{
		m_shader->SetTechnique(technique);
		m_shader->Begin(nullptr, NULL);
	}
}