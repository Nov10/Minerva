#include "VertexShader.h"
#include <cmath>

void VertexShader::Initialize(int programID, const char* kernelName_Transform)
{
    KernelID_Transform = GPUControl.LoadKernel(kernelName_Transform, programID);
}

void VertexShader::Execute(int length, int bufferID_vertices, int bufferID_normals, int bufferID_transformedVertices, int bufferID_transformedNormals,
    Matrix4x4* M, Matrix4x4* VP, Matrix4x4* R, int* width, int* height)
{
    size_t sizes[] = {
        sizeof(cl_mem), sizeof(cl_mem),
        sizeof(Matrix4x4), sizeof(Matrix4x4), sizeof(Matrix4x4), sizeof(int), sizeof(int),
        sizeof(cl_mem), sizeof(cl_mem)
    };

    const void* args[] = {
        GPUControl.GetBufferPtr(bufferID_vertices), GPUControl.GetBufferPtr(bufferID_normals),
        M, VP, R, width, height,
        GPUControl.GetBufferPtr(bufferID_transformedVertices), GPUControl.GetBufferPtr(bufferID_transformedNormals)
    };

    GPUControl.ExecuteKernel(KernelID_Transform, length, 9, sizes, args);
}