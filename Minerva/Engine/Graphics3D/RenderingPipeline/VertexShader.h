#pragma once
#include "../../MMath/MMath.h"
#include "../../GPU/OpenCL/OpenCLController.h"

class VertexShader
{
public:
	int KernelID_Transform;
	void Initialize(int programID, const char*  kernelName_Transform)
	{
		KernelID_Transform = GPUControl.LoadKernel(kernelName_Transform, programID);

	}
	void Execute(int length, int bufferID_vertices, int bufferID_normals, int bufferID_transformedVertices, int bufferID_transformedNormals,
		
		Matrix4x4* M, Matrix4x4* VP, Matrix4x4* R, int* width, int* height)
	{
		size_t sizes[] = {
			sizeof(cl_mem), sizeof(cl_mem),
			sizeof(Matrix4x4), sizeof(Matrix4x4), sizeof(Matrix4x4), sizeof(int), sizeof(int),
			sizeof(cl_mem), sizeof(cl_mem) };

		const void* args[] = {
		GPUControl.GetBufferPtr(bufferID_vertices), GPUControl.GetBufferPtr(bufferID_normals),
			(M), (VP), (R), width, height,
		GPUControl.GetBufferPtr(bufferID_transformedVertices), GPUControl.GetBufferPtr(bufferID_transformedNormals) };

		GPUControl.ExecuteKernel(KernelID_Transform, length, 9, sizes, args);
	}

	void Convert_LocalPos2WorldPos(const Vector3* const localPos, Vector3* worldPos, Matrix4x4* M)
	{
		*worldPos = M->transform(*localPos);
	}
	void Convert_WorldPos2ClipSpace(Vector3* worldPos, Vector4* clipSpace, Matrix4x4* VP)
	{
		*clipSpace = (*VP) * (Vector4(worldPos->x, worldPos->y, worldPos->z, 1));
	}
	void Convert_WorldClip2ScreenSpace(Vector4* clipPos, Vector3* screenSpace, const int* width, const int* height)
	{
		*clipPos = (*clipPos) / (clipPos->w);

		*screenSpace = Vector3((-clipPos->x + 1) * 0.5f * (*width), (-clipPos->y + 1) * 0.5f * (*height), clipPos->z);

		(*screenSpace).x = floor(screenSpace->x);
		(*screenSpace).y = floor(screenSpace->y);
	}
	void Apply_LocalPos2WorldPos(int length, const Vector3* const localPosition,
		Vector3* screenPosition, Matrix4x4* M, Matrix4x4* VP, const int* width, const int* height)
	{
		Vector3* worldSpace = new Vector3[length];
		Vector4* clipSpace = new Vector4[length];
		for (int i = 0; i < length; i++)
		{
			Vector3 pos_LocalSpace = localPosition[i];
			Convert_LocalPos2WorldPos(&(localPosition[i]), &(worldSpace[i]), M);
			Convert_WorldPos2ClipSpace(&(worldSpace[i]), &(clipSpace[i]), VP);
			Convert_WorldClip2ScreenSpace(&(clipSpace[i]), &(screenPosition[i]), width, height);
		}
		delete[] worldSpace;
		delete[] clipSpace;
	}

};