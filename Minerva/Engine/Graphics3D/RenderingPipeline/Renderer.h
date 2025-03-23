#pragma once
#include <vector>
#include "../MeshRenderer.h"
#include "../RenderData.h"
#include "../Camera.h"
#include "VertexShader.h"
#include "Rasterizer.h"

class Renderer
{
private:
	std::vector<Camera*> Cameras;
	//std::vector<MeshRenderer*> RenderedTargets;
	VertexShader VTXShader;
	Rasterizer ThisRasterizer;
	//int TileCount;
	//int TileSize;
	void RenderOnSingleCamera(Camera* cam);
public:

	void Initialize();
	void Render();

	void AddCamera(Camera* cam);
	void RemoveCamera(Camera* cam);
};