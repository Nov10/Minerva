#include "EngineController.h"

EngineController::EngineController(int width, int height) : ResultImage(width, height)
{

}

float EngineController::Render()
{
	NowPriority = 0;
	for (auto& r : Renderers)
	{
		(*r).Render();
	}
	return 0.0f;
}

void EngineController::Update()
{
	for (auto& scene : Scenes)
	{
		for (auto& go : scene->GameObjects)
		{
			go->Start_ENGINE();
		}
	}
	for (auto& scene : Scenes)
	{
		for (auto& go : scene->GameObjects)
		{
			go->Update_ENGINE();
		}
	}
	for (auto& Scene : Scenes)
	{
		for (auto& go : Scene->GameObjects)
		{
			go->LateUpdate_ENGINE();
		}
	}
	Render();
}
