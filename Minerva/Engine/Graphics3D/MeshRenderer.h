#pragma once
#include <vector>
#include "../MMath/MMath.h"
#include "RenderData.h"
#include "../Core/Component/Component.h"

#include "RenderingPipeline/MeshRendererContainer.h"

class MeshRenderer : public Component
{
public:
	std::vector<RenderData> Datas;
	MeshRenderer() {
		MeshRendererContainer::Instance().AddRenderer(this);
	}
	~MeshRenderer() {
		MeshRendererContainer::Instance().RemoveRenderer(this);
	}
};