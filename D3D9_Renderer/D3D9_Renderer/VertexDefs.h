#pragma once
#include <d3d9.h>
#include <DirectXMath.h>

namespace renderer
{
    class VertexDeclContainer
    {
    public:
        IDirect3DVertexDeclaration9* positionVertexDecl = nullptr;
    };

    struct PositionVertex
    {
        PositionVertex()
			:m_position({ 0.0f, 0.0f, 0.0f, 0.0f }),
			m_normal({ 0.0f, 0.0f, 0.0f, 0.0f })
        {}
        PositionVertex(float vx, float vy, float vz, float nx, float ny, float nz)
            :m_position({ vx, vy, vz , 0.0f}),
			m_normal({nx, ny, nz})
        {}
        PositionVertex(const DirectX::XMVECTOR& position, const DirectX::XMVECTOR& normal)
        {
			m_position = position;
			m_normal = normal;
		}

		DirectX::XMVECTOR m_position;
		DirectX::XMVECTOR m_normal;
    };
}