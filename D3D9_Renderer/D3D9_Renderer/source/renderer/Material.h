#pragma once

#include <string>
#include <d3d9.h>

#include"d3d9/Shader.h"

namespace renderer
{
    class Material
    {
    public:
        Material();
        ~Material();

    private:
        Shader m_shader;
        
        IDirect3DTexture9* m_diffuseTexture;
        IDirect3DTexture9* m_normalTexture;
    };
}
