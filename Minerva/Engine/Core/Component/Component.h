#pragma once
//#include "../Object/GameObject.h"

class GameObject;

class Component
{
public:
	std::weak_ptr<GameObject> Owner;
	Component() { IsActive = true; }
	virtual ~Component() {}
	virtual void Update() {};
	virtual void LateUpdate() {};

	bool IsActive;
	void SetActive(bool active) { IsActive = active; }
};