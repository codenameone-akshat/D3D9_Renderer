#include "Material.h"
#include "../utils/ComHelpers.h"
namespace renderer
{
    Material::Material()
        :m_diffuseTexture(),
        m_normalTexture(),
		m_vBufferBatchOffset(0)
    {
    }

    Material::~Material()
    {
        ComSafeRelease(m_diffuseTexture);
        ComSafeRelease(m_normalTexture);
    }
    void Material::SetTexture(TextureType texType, IDirect3DTexture9* texture)
    {
        switch (texType)
        {
        case TextureType::Diffuse:
            m_diffuseTexture = texture;
            break;
        case TextureType::Normal:
            m_normalTexture = texture;
            break;
        default:
            break;
        }
    }
    IDirect3DTexture9* Material::GetTextureOfType(TextureType texType)
    {
        switch (texType)
        {
        case TextureType::Diffuse:
            return m_diffuseTexture;
            break;
        case TextureType::Normal:
            return m_normalTexture;
            break;
        default:
            return nullptr;
            break;
        }
    }
    IDirect3DTexture9** Material::GetPtrToTextureOfType(TextureType texType)
    {
        switch (texType)
        {
        case TextureType::Diffuse:
            return &m_diffuseTexture;
            break;
        case TextureType::Normal:
            return &m_normalTexture;
            break;
        default:
            return nullptr;
            break;
        }
    }
}