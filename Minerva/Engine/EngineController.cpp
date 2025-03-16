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
	Render();
}
