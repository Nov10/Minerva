#pragma once
#include "../GPU/GPUController.h"
#include "../GPU/OpenCL/OpenCLController.h"
#include "../Debug.h"

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

public:
	void Initialize(int programID, const char* kernelName_CalculateCacheTrianglesPerTile,
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

		BufferID_ZBuffer = GPUControl.CreateBuffer(CL_MEM_READ_WRITE, sizeof(float)  * width * height, nullptr);
		BufferID_TrianglesIndicidesPerTile = GPUControl.CreateBuffer(CL_MEM_READ_WRITE, sizeof(int) * quadCountWidth * quadCountHeight * maxTriangleCountPerTile, nullptr);
		BufferID_TriangleCountPerTile = GPUControl.CreateBuffer(CL_MEM_READ_WRITE, sizeof(int) * quadCountWidth * quadCountHeight, nullptr);
	}
	void Ready(Camera* cam)
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
			BufferID_ZBuffer = GPUControl.CreateBuffer(CL_MEM_READ_WRITE, sizeof(float) * (cam->Width) * (cam->Height), nullptr);

			MaxWidth = cam->Width;
			MaxHeight = cam->Height;
		}
		float z0 = 10000000;
		GPUControl.SetBufferDatas(BufferID_ZBuffer, &z0, sizeof(float)* (cam->Width)* (cam->Height));


		int zeroValue = 0;
		GPUControl.SetBufferDatas(BufferID_TriangleCountPerTile, &zeroValue, sizeof(int) * quadCountWidth * quadCountHeight);
		GPUControl.Finish();
	}
	void Execute(const int buffefID_vertices, const int bufferID_triangles, const int bufferID_normals, const int bufferID_frameBuffer, const int trianglesLength, const int* width, const int* height, const Vector3* light)
	{
		int quadCountWidth = *width / QuadSize;
		int quadCountHeight = *height / QuadSize;


		size_t sizes2[] = 
		{ sizeof(cl_mem), sizeof(cl_mem), 
		  sizeof(cl_mem), sizeof(cl_mem),
		  sizeof(int), sizeof(int),											 sizeof(int), sizeof(int) };
		const void* args2[] = 
		{ GPUControl.GetBufferPtr(BufferID_TrianglesIndicidesPerTile), GPUControl.GetBufferPtr(BufferID_TriangleCountPerTile),
		  GPUControl.GetBufferPtr(buffefID_vertices),                  GPUControl.GetBufferPtr(bufferID_triangles),
		  width, height,													 &QuadSize, &MaxTriangleCountPerQuad };

		//Debug::Log("BB");
		//int* tmp = new int[quadCountWidth * quadCountHeight];
		////GPUControl.ReadBuffer(BufferID_TriangleCountPerTile, sizeof(int) * quadCountWidth * quadCountHeight, tmp);
		//for (int s = 0; s < quadCountWidth * quadCountHeight; s++)
		//{
		//	//if(tmp[s] > 3)
		//		//Debug::Log(tmp[s]);
		//}

		//delete[] tmp;
		GPUControl.ExecuteKernel(KernelID_CalculateCacheTrianglesPerTile, trianglesLength, 8, sizes2, args2);

		size_t sizes3[] =
		{ sizeof(cl_mem),													 sizeof(cl_mem), 
		  sizeof(cl_mem),													 sizeof(cl_mem),  sizeof(cl_mem),
		  sizeof(cl_mem), sizeof(cl_mem),
		  sizeof(int), sizeof(int),											 sizeof(int), sizeof(int),
		sizeof(Vector3)};
		const void* args3[] = 
		{ GPUControl.GetBufferPtr(BufferID_TrianglesIndicidesPerTile), GPUControl.GetBufferPtr(BufferID_TriangleCountPerTile),
		  GPUControl.GetBufferPtr(buffefID_vertices),                  GPUControl.GetBufferPtr(bufferID_normals), GPUControl.GetBufferPtr(bufferID_triangles),
		  GPUControl.GetBufferPtr(bufferID_frameBuffer), GPUControl.GetBufferPtr(BufferID_ZBuffer),
		  width, height,													 &QuadSize, &MaxTriangleCountPerQuad,
		 light };

		GPUControl.ExecuteKernel(KernelID_CalculateRastersPerTile, quadCountWidth * quadCountHeight, 12, sizes3, args3);
		//GPUControl.Finish();
	}
};