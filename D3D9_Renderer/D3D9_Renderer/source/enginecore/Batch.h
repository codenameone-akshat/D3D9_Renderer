#pragma once

namespace renderer
{
    struct BatchDesc
    {
        BatchDesc()
            :primitiveCount(0),
            vertexCount(0),
            indexStart(0)
        {}

        uint32_t primitiveCount;
        uint32_t vertexCount;
        uint32_t indexStart;
    };
}