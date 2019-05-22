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
        
    private:
        ID3DXEffect* m_shader;
        std::string m_shaderPath;
        D3DXEFFECT_DESC m_shaderDesc;
        std::map<D3DXHANDLE, D3DXTECHNIQUE_DESC> m_techniques;
    };
}

