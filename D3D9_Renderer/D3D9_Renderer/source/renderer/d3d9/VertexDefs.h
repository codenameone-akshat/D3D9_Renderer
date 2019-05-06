#pragma once

#include <d3d9.h>

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
			:m_vx(0.0f),
            m_vy(0.0f),
            m_vz(0.0f),
            m_nx(0.0f),
            m_ny(0.0f),
            m_nz(0.0f)
        {}
        PositionVertex(float vx, float vy, float vz, float nx, float ny, float nz)
            :m_vx(vx),
            m_vy(vy),
            m_vz(vz),
            m_nx(nx),
            m_ny(ny),
            m_nz(nz)
        {}
        float m_vx, m_vy, m_vz;
        float m_nx, m_ny, m_nz;
    };
}