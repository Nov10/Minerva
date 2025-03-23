#include "Renderer.h"
#include "../../MMath/MMath.h"
#include "../../Graphics2D/MBitmap.h"
#include "../../Graphics2D/MColor.h"
#include "../../GPU/OpenCL/OpenCLController.h"
#include "../../Timer.h"
#include "../../../Engine/Debug.h"
#include "../../../Engine/EngineController.h"
#include "MeshRendererContainer.h"

Quaternion lightDir;
void Renderer::Initialize()
{
	std::string kernels_vertexshader[] = { "Engine/GPU/OpenCL/CL_Maths.cl", "Engine/GPU/OpenCL/CL_VP_VertexShader.cl" };
	int ProgramID_VertexShader = GPUControl.LoadKernelProgram(kernels_vertexshader, 2);
	VTXShader = VertexShader();
	VTXShader.Initialize(ProgramID_VertexShader, "TransformVertices");



	std::string kernels_rasterizer[] = { "Engine/GPU/OpenCL/CL_Maths.cl", "Engine/GPU/OpenCL/CL_Rasterizer.cl" };
	int ProgramID_rasterizer = GPUControl.LoadKernelProgram(kernels_rasterizer, 2);
	//ThisRasterizer = Rasterizer();
	ThisRasterizer.Initialize(ProgramID_rasterizer, "CalculateCacheTrianglesPerTile", "CalculateRastersPerTile", 4, 4, 2, 64);
}
void Renderer::Render()
{
	//for (auto iter = Cameras.begin(); iter != Cameras.end(); iter++) {
	for(auto& cam : Cameras)
	{
		RenderOnSingleCamera(cam);
	}
}

void DrawLine(MBitmap& frameBuffer, int x0, int y0, int x1, int y1, MColor color) {
	int dx = abs(x1 - x0);
	int dy = abs(y1 - y0);
	int sx = (x0 < x1) ? 1 : -1;
	int sy = (y0 < y1) ? 1 : -1;
	int err = dx - dy;

	while (true) {
		frameBuffer.Set(x0, y0, color); // 픽셀 설정

		if (x0 == x1 && y0 == y1) break; // 종료 조건

		int e2 = 2 * err;
		if (e2 > -dy) {
			err -= dy;
			x0 += sx;
		}
		if (e2 < dx) {
			err += dx;
			y0 += sy;
		}
	}
}

void Renderer::RenderOnSingleCamera(Camera* cam)
{
	ThisRasterizer.Ready(cam);
	(*cam).Target.Clear(MColor(0, 0, 0));

	//RenderData* d = nullptr;
	Matrix4x4 VP = cam->CreateVPMatrix();
	int width = cam->Width;
	int height = cam->Height;

	MBitmap frameBuffer = MBitmap(width, height);	
	MColor* fBuffer = new MColor[width * height];
	for (int i = 0; i < width * height; i++)
	{
		fBuffer[i] = MColor(0, 0, 0);
	}

	int bufferFrameBuffer = GPUControl.CreateBuffer(CL_MEM_READ_WRITE | CL_MEM_COPY_HOST_PTR, sizeof(MColor) * width * height, fBuffer);


	lightDir = Quaternion::FromEulerAngles(-30, 30, 0);
	Vector3 light = lightDir.RotateVector(Vector3(0, 0, -1)).normalized();

	//for (auto iter = RenderedTargets.begin(); iter != RenderedTargets.end(); iter++)
	std::vector<MeshRenderer*> v = MeshRendererContainer::Instance().Renderers;
	for (auto& t : v)
	{
		if (t->IsActive == false)
			continue;

		auto owner = t->Owner.lock();
		if (owner->IsActiveInScene() == false)
			continue;
		Matrix4x4 M = CreateTransformMatrix(owner.get()->GetWorldPosition(), owner.get()->GetWorldRotation(), owner.get()->GetWorldScale());
		Matrix4x4 R = CreateRotationMatrix(owner.get()->GetWorldRotation());
		for (auto& d : t->Datas)
		{
			int length = d.GetVertices_Length();
			int trianglesLength = d.GetTriangles_Length();
			int bufferVectors = GPUControl.CreateBuffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(Vector3) * length, d.Vertices);
			int bufferNormals = GPUControl.CreateBuffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(Vector3) * length, d.Normals);
			int bufferTriangles = GPUControl.CreateBuffer(CL_MEM_READ_ONLY | CL_MEM_COPY_HOST_PTR, sizeof(int) * trianglesLength, d.Triangles);
			int bufferResults = GPUControl.CreateBuffer(CL_MEM_READ_WRITE, sizeof(Vector3) * length, nullptr);
			int bufferResultNormals = GPUControl.CreateBuffer(CL_MEM_READ_WRITE, sizeof(Vector3) * length, nullptr);

			VTXShader.Execute(length, bufferVectors, bufferNormals, bufferResults, bufferResultNormals, &M, &VP, &R, &width, &height);
			ThisRasterizer.Execute(bufferResults, bufferTriangles, bufferResultNormals, bufferFrameBuffer, trianglesLength, &width, &height, &light);


			GPUControl.ReleaseBuffer(bufferVectors);
			GPUControl.ReleaseBuffer(bufferNormals);
			GPUControl.ReleaseBuffer(bufferTriangles);
			GPUControl.ReleaseBuffer(bufferResults);
			GPUControl.ReleaseBuffer(bufferResultNormals);

		}
			GPUControl.Finish();
	}
	GPUControl.ReadBuffer(bufferFrameBuffer, sizeof(MColor) * width * height, fBuffer);
	for (int x = 0; x < width; x++)
	{
		for (int y = 0; y < height; y++)
		{
			frameBuffer.Set(x, y, fBuffer[x + y * width]);
		}
	}
	cam->Target.Set(frameBuffer);

	if (cam->IsMainGameCamera == true)
	{
		MinervaEngine.SetResultImage(frameBuffer, 0);
	}

	delete[] fBuffer;
	GPUControl.ReleaseBuffer(bufferFrameBuffer);
}

void Renderer::AddCamera(Camera* cam)
{
	Cameras.emplace_back(cam);
}

void Renderer::RemoveCamera(Camera* cam)
{
}