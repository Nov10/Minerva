#pragma once
//#include "../Object/GameObject.h"

class GameObject;

class Component
{
public:
	std::weak_ptr<GameObject> Owner;
	Component() { IsActive = true; }
	virtual ~Component() {}
    void Start_ENGINE() {
		IsStarted = true;
		Start();
	};
	void Update_ENGINE() {
		Update();
	};
	void LateUpdate_ENGINE() {
		LateUpdate();
	};
	bool IsStarted = false;
	bool IsActive;
	void SetActive(bool active) { IsActive = active; }
protected:
	virtual void Start() {};
	virtual void Update() {};
	virtual void LateUpdate() {};
};