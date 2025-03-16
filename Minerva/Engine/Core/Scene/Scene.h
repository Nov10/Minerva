#pragma once
#include "../Object/GameObject.h"
#include <vector>
class Scene
{
public:
	std::vector<std::shared_ptr<GameObject>> GameObjects;
};