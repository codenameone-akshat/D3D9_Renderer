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
            :_position({ 0.0f, 0.0f, 0.0f, 0.0f })
        {}
        PositionVertex(float x, float y, float z)
            :_position({ x, y, z , 0.0f})
        {}
        PositionVertex(const DirectX::XMVECTOR& position)
            :_position(position)
        {}

        DirectX::XMVECTOR _position;
    };
}