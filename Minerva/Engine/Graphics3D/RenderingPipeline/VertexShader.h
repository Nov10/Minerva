#pragma once

#include "../../MMath/MMath.h"
#include "../../GPU/OpenCL/OpenCLController.h"

class VertexShader
{
public:
    int KernelID_Transform;

    void Initialize(int programID, const char* kernelName_Transform);
    void Execute(int length, int bufferID_vertices, int bufferID_normals, int bufferID_transformedVertices, int bufferID_transformedNormals,
        Matrix4x4* M, Matrix4x4* VP, Matrix4x4* R, int* width, int* height);
};
