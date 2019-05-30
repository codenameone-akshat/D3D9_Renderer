#pragma once

#include <d3dx9.h>
#include <string>
#include <map>

namespace renderer
{
    class Shader
    {
    public:
        Shader();
        ~Shader();
        
        void CreateShader(IDirect3DDevice9* device, std::string shaderPath);
		void SetTechniqueAndBegin(D3DXHANDLE technique);
		inline void BeginPass(uint32_t passIndex) { m_shader->BeginPass(passIndex); }
		inline void ApplyPass() { m_shader->CommitChanges(); }
		inline void EndPass() { m_shader->EndPass(); }
		inline void EndTechnique() { m_shader->End(); }


		std::map<D3DXHANDLE, D3DXTECHNIQUE_DESC> GetTechniqueData() const { return m_techniques; }
		[[nodiscard]] ID3DXEffect* GetRawPtr() { return m_shader; }

    private:
        ID3DXEffect* m_shader;
        std::string m_shaderPath;
        D3DXEFFECT_DESC m_shaderDesc;
        std::map<D3DXHANDLE, D3DXTECHNIQUE_DESC> m_techniques;
    };
}

