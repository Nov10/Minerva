#pragma once
#include "Graphics2D/Minerva2DGraphics.h"
#include "Graphics3D/Minerva3DGraphics.h"
#include "MMath/MMath.h"
#include "GPU/GPUController.h"
#include "Timer.h"
#include "Debug.h"
#include "Core/Object/Object.h"
#include "Core/Object/GameObject.h"
#include "Core/Scene/Scene.h"

class EngineController
{
public:
	EngineController(int width, int height);
	std::vector<std::shared_ptr<Scene>> Scenes;

	float Render();
	void Update();
	RenderTarget* GetRenderedResult() { return static_cast<RenderTarget*>(&ResultImage); }
	void AddRenderer(Renderer* r)
	{
		Renderers.emplace_back(r);
	}
	void SetResultImage(const MBitmap& image, int priority)
	{
		if (priority >= NowPriority)
		{
			if (image.GetHeight() != ResultImage.GetHeight() || image.GetWidth() != ResultImage.GetWidth())
			{
				ResultImage = RenderTarget(image.GetWidth(), image.GetHeight());
			}
			ResultImage.Set(image);
			NowPriority = priority;
		}
	}
private:
	int NowPriority;
	RenderTarget ResultImage;
	std::vector<Renderer*> Renderers;
};

extern EngineController engine;