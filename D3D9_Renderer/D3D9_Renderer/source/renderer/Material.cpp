#include "Material.h"
#include "../utils/ComHelpers.h"
namespace renderer
{
    Material::Material()
        :m_diffuseTexture(nullptr),
        m_normalTexture(nullptr),
		m_specularTexture(nullptr),
		m_opacityTexture(nullptr)
    {
    }

    Material::~Material()
    {
        ComSafeRelease(m_diffuseTexture);
		ComSafeRelease(m_normalTexture);
		ComSafeRelease(m_specularTexture);
		ComSafeRelease(m_opacityTexture);
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
		case TextureType::Specular:
			m_specularTexture = texture;
			break;
		case TextureType::Opacity:
			m_opacityTexture = texture;
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
		case TextureType::Specular:
			return m_specularTexture;
			break;
		case TextureType::Opacity:
			return m_opacityTexture;
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
		case TextureType::Specular:
			return &m_specularTexture;
			break;
		case TextureType::Opacity:
			return &m_opacityTexture;
			break;
        default:
            return nullptr;
            break;
        }
    }
}