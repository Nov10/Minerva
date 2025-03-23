#pragma once

#include "../../GPU/GPUController.h"
#include "../../GPU/OpenCL/OpenCLController.h"
#include "../../Debug.h"
#include "../Camera.h"

class Rasterizer
{
private:
    int KernelID_CalculateCacheTrianglesPerTile;
    int KernelID_CalculateRastersPerTile;

    int BufferID_TrianglesIndicidesPerTile;
    int BufferID_TriangleCountPerTile;
    int BufferID_ZBuffer;

    int MaxWidth;
    int MaxHeight;
    int QuadSize;
    int MaxTriangleCountPerQuad;


    const float DefaultDepth = FLT_MAX;
    int DefaultTriangleCountPerTile = 0;

public:
    Rasterizer() = default;

    void Initialize(int programID,
        const char* kernelName_CalculateCacheTrianglesPerTile,
        const char* kernelName_CalculateRastersPerTile,
        int width, int height, int quadSize, int maxTriangleCountPerTile);

    void Ready(Camera* cam);

    void Execute(const int buffefID_vertices, const int bufferID_triangles, const int bufferID_normals,
        const int bufferID_frameBuffer, const int trianglesLength,
        const int* width, const int* height, const Vector3* light);
};
