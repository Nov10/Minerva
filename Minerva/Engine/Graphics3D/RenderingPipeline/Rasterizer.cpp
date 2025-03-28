#include "Rasterizer.h"

void Rasterizer::Initialize(int programID,
    const char* kernelName_CalculateCacheTrianglesPerTile,
    const char* kernelName_CalculateRastersPerTile,
    int width, int height, int quadSize, int maxTriangleCountPerTile)
{
    KernelID_CalculateCacheTrianglesPerTile = GPUControl.LoadKernel(kernelName_CalculateCacheTrianglesPerTile, programID);
    KernelID_CalculateRastersPerTile = GPUControl.LoadKernel(kernelName_CalculateRastersPerTile, programID);

    MaxWidth = width;
    MaxHeight = height;
    QuadSize = quadSize;
    MaxTriangleCountPerQuad = maxTriangleCountPerTile;
    int quadCountWidth = width / quadSize;
    int quadCountHeight = height / quadSize;

    BufferID_ZBuffer = GPUControl.CreateBuffer(CL_MEM_READ_WRITE, sizeof(float) * width * height, nullptr);
    BufferID_TrianglesIndicidesPerTile = GPUControl.CreateBuffer(CL_MEM_READ_WRITE, sizeof(int) * quadCountWidth * quadCountHeight * maxTriangleCountPerTile, nullptr);
    BufferID_TriangleCountPerTile = GPUControl.CreateBuffer(CL_MEM_READ_WRITE, sizeof(int) * quadCountWidth * quadCountHeight, nullptr);
}

void Rasterizer::Ready(Camera* cam)
{
    int quadCountWidth = cam->Width / QuadSize;
    int quadCountHeight = cam->Height / QuadSize;
    if (cam->Width > MaxWidth || cam->Height > MaxHeight)
    {
        Debug::Log("Re Allocate Buffer");
        GPUControl.ReleaseBuffer(BufferID_TrianglesIndicidesPerTile);
        GPUControl.ReleaseBuffer(BufferID_TriangleCountPerTile);
        GPUControl.ReleaseBuffer(BufferID_ZBuffer);
        BufferID_TrianglesIndicidesPerTile = GPUControl.CreateBuffer(CL_MEM_READ_WRITE, sizeof(int) * quadCountWidth * quadCountHeight * MaxTriangleCountPerQuad, nullptr);
        BufferID_TriangleCountPerTile = GPUControl.CreateBuffer(CL_MEM_READ_WRITE, sizeof(int) * quadCountWidth * quadCountHeight, nullptr);
        BufferID_ZBuffer = GPUControl.CreateBuffer(CL_MEM_READ_WRITE, sizeof(float) * cam->Width * cam->Height, nullptr);

        MaxWidth = cam->Width;
        MaxHeight = cam->Height;
    }

    GPUControl.SetBufferDatas(BufferID_ZBuffer, &DefaultDepth, sizeof(float) * cam->Width * cam->Height);

    GPUControl.SetBufferDatas(BufferID_TriangleCountPerTile, &DefaultTriangleCountPerTile, sizeof(int) * quadCountWidth * quadCountHeight);
    GPUControl.Finish();
}

void Rasterizer::Execute(const int buffefID_vertices, const int bufferID_triangles, const int bufferID_normals,
    const int bufferID_frameBuffer, const int trianglesLength,
    const int* width, const int* height, const Vector3* light)
{
    int quadCountWidth = *width / QuadSize;
    int quadCountHeight = *height / QuadSize;

    GPUControl.SetBufferDatas(BufferID_TriangleCountPerTile, &DefaultTriangleCountPerTile, sizeof(int) * quadCountWidth * quadCountHeight);

    size_t sizes2[] = { sizeof(cl_mem), sizeof(cl_mem),
                        sizeof(cl_mem), sizeof(cl_mem),
                        sizeof(int), sizeof(int), sizeof(int), sizeof(int) };
    const void* args2[] = { GPUControl.GetBufferPtr(BufferID_TrianglesIndicidesPerTile),
                            GPUControl.GetBufferPtr(BufferID_TriangleCountPerTile),
                            GPUControl.GetBufferPtr(buffefID_vertices),
                            GPUControl.GetBufferPtr(bufferID_triangles),
                            width, height, &QuadSize, &MaxTriangleCountPerQuad };

    GPUControl.ExecuteKernel(KernelID_CalculateCacheTrianglesPerTile, trianglesLength, 8, sizes2, args2);

    size_t sizes3[] = { sizeof(cl_mem), sizeof(cl_mem),
                        sizeof(cl_mem), sizeof(cl_mem), sizeof(cl_mem),
                        sizeof(cl_mem), sizeof(cl_mem), sizeof(int), sizeof(int),
                        sizeof(int), sizeof(int), sizeof(Vector3) };
    const void* args3[] = { GPUControl.GetBufferPtr(BufferID_TrianglesIndicidesPerTile),
                            GPUControl.GetBufferPtr(BufferID_TriangleCountPerTile),
                            GPUControl.GetBufferPtr(buffefID_vertices),
                            GPUControl.GetBufferPtr(bufferID_normals),
                            GPUControl.GetBufferPtr(bufferID_triangles),
                            GPUControl.GetBufferPtr(bufferID_frameBuffer),
                            GPUControl.GetBufferPtr(BufferID_ZBuffer),
                            width, height, &QuadSize, &MaxTriangleCountPerQuad,
                            light };

    GPUControl.ExecuteKernel(KernelID_CalculateRastersPerTile, quadCountWidth * quadCountHeight, 12, sizes3, args3);
    //GPUControl.Finish();
}
