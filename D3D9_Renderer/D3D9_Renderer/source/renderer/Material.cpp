#include "Material.h"
#include "../utils/ComHelpers.h"
namespace renderer
{
    Material::Material()
        :m_shader(),
        m_diffuseTexture(nullptr),
        m_normalTexture(nullptr)
    {
    }

    Material::~Material()
    {
        ComSafeRelease(m_diffuseTexture);
        ComSafeRelease(m_normalTexture);
    }
}