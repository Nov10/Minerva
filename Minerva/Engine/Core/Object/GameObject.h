#pragma once
#include "Object.h"
#include "../../MMath/MMath.h"
#include "../Component/Component.h"
#include "../Scene/Scene.h"


class GameObject : public Object, public std::enable_shared_from_this<GameObject>
{
private:
	Vector3 LocalPosition;
	Quaternion LocalRotation;
	Vector3 LocalScale;
	std::shared_ptr<GameObject> Parent;
	std::vector<std::shared_ptr<Component>> Components;
	bool IsActive;

public:

	static std::shared_ptr<GameObject> CreateGameObject(Scene& target)
	{
		auto go = std::make_shared<GameObject>();
		go->IsActive = true;
		target.GameObjects.emplace_back(go);
		return go;
	}
	GameObject() : LocalPosition(Vector3(0, 0, 0)), LocalRotation(Quaternion(0, 0, 0, 1)), LocalScale(Vector3(1, 1, 1)) {

	}
	GameObject(Vector3 position, Quaternion rotation, Vector3 scale) : LocalPosition(position), LocalRotation(rotation), LocalScale(scale) {}
	void SetActive(bool active) { IsActive = active; }
	Vector3 GetLocalPosition() const { return LocalPosition; }
	Quaternion GetLocalRotation() const { return LocalRotation; }
	Vector3 GetLocalScale() const { return LocalScale; }
	void SetLocalPosition(const Vector3& position) { LocalPosition = position; }
	void SetLocalRotation(const Quaternion& rotation) { LocalRotation = rotation; }
	void SetLocalScale(const Vector3& scale) { LocalScale = scale; }
	void SetParent(std::shared_ptr<GameObject> parent) { Parent = parent; }
	std::shared_ptr<GameObject> GetParent() const { return Parent; }
	Vector3 GetWordPosition() const
	{
		if (Parent != nullptr)
		{
			Vector3 v = Parent->GetWordPosition();
			return v + Parent->GetWordRotation().RotateVector(LocalPosition);
		}
		else
		{
			return LocalPosition;
		}
	}
	Quaternion GetWordRotation() const
	{
		if (Parent != nullptr)
		{
			return Parent->GetWordRotation() * LocalRotation;
		}
		else
		{
			return LocalRotation;
		}
	}
	Vector3 GetWorldScale() const
	{
		if (Parent != nullptr)
		{
			return Vector3::ElemetProduct(Parent->GetWorldScale(), LocalScale);
		}
		else
		{
			return LocalScale;
		}
	}

	bool IsActiveInScene() const
	{
		if (IsActive == false)
		{
			return false;
		}
		else
		{
			if (Parent != nullptr)
			{
				return Parent->IsActiveInScene();
			}
			return true;
		}
	}

	void SetWorldPosition(const Vector3& position)
	{
		if (Parent != nullptr)
		{
			Vector3 parentWorldPosition = Parent->GetWordPosition();
			Vector3 diff = position - parentWorldPosition;
			LocalPosition = diff;
		}
		else
		{
			LocalPosition = position;
		}
	}
	void SetWorldRotation(const Quaternion& rotation)
	{
		if (Parent != nullptr)
		{
			Quaternion parentWorldRotation = Parent->GetWordRotation();
			Quaternion diff = parentWorldRotation.Inverse() * rotation;
			LocalRotation = diff;
		}
		else
		{
			LocalRotation = rotation;
		}
	}
	void SetWorldScale(const Vector3& scale)
	{
		if (Parent != nullptr)
		{
			Vector3 parentWorldScale = Parent->GetWorldScale();
			Vector3 diff = Vector3::ElemetDivision(scale, parentWorldScale);
			LocalScale = diff;
		}
		else
		{
			LocalScale = scale;
		}
	}


	template<typename T>
	std::shared_ptr<T> AddComponent()
	{
		std::shared_ptr<T> component = std::make_shared<T>();
		component.get()->Owner = shared_from_this();
		//shared_from_this();

		Components.push_back(component);
		return component;
	}

	void Update()
	{
		for (auto& component : Components)
		{
			component->Update();
		}
	}

	void LateUpdate()
	{
		for (auto& component : Components)
		{
			component->LateUpdate();
		}
	}
};