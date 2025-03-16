#pragma once

#include <vector>
//#include "../MeshRenderer.h"

class MeshRenderer;

class MeshRendererContainer
{
public:
	std::vector<MeshRenderer*> Renderers;

	static MeshRendererContainer& Instance() {
		static MeshRendererContainer instance;
		return instance;
	}
	MeshRendererContainer() {}
	void AddRenderer(MeshRenderer* r)
	{
		Renderers.emplace_back(r);
	}
	void RemoveRenderer(MeshRenderer* r)
	{
		Renderers.erase(std::remove(Renderers.begin(), Renderers.end(), r), Renderers.end());
	}
};