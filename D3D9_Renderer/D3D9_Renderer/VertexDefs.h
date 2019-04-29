#pragma once
#include <d3d9.h>

namespace d3dgfx
{
    class VertexDeclContainer
    {
    public:
        IDirect3DVertexDeclaration9* positionVertexDecl = nullptr; //init static var
    };

    struct PositionVertex
    {
        PositionVertex()
            :_position({ 0.0f, 0.0f, 0.0f })
        {}
        PositionVertex(float x, float y, float z)
            :_position({ x, y, z })
        {}
        PositionVertex(const D3DVECTOR& position)
            :_position(position)
        {}

        D3DVECTOR _position;
    };
}