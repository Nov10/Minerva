#pragma once
#include "GameObject.h"

// 생성자 구현
GameObject::GameObject()
    : LocalPosition(Vector3(0, 0, 0)),
    LocalRotation(Quaternion(0, 0, 0, 1)),
    LocalScale(Vector3(1, 1, 1)),
    Parent(nullptr),
    IsActive(false)
{
}

GameObject::GameObject(Vector3 position, Quaternion rotation, Vector3 scale)
    : LocalPosition(position),
    LocalRotation(rotation),
    LocalScale(scale),
    Parent(nullptr),
    IsActive(false)
{
}

// 정적 생성 함수 구현
std::shared_ptr<GameObject> GameObject::CreateGameObject(Scene& target)
{
    auto go = std::make_shared<GameObject>();
    go->IsActive = true;
    target.GameObjects.emplace_back(go);
    return go;
}

// 멤버 함수 구현
void GameObject::SetActive(bool active)
{
    IsActive = active;
}

Vector3 GameObject::GetLocalPosition() const
{
    return LocalPosition;
}

Quaternion GameObject::GetLocalRotation() const
{
    return LocalRotation;
}

Vector3 GameObject::GetLocalScale() const
{
    return LocalScale;
}

void GameObject::SetLocalPosition(const Vector3& position)
{
    LocalPosition = position;
}

void GameObject::SetLocalRotation(const Quaternion& rotation)
{
    LocalRotation = rotation;
}

void GameObject::SetLocalScale(const Vector3& scale)
{
    LocalScale = scale;
}

void GameObject::SetParent(std::shared_ptr<GameObject> parent)
{
	Vector3 originalPosition = GetWorldPosition();
	Quaternion originalRotation = GetWorldRotation();
	Vector3 originalScale = GetWorldScale();
    Parent = parent;
	SetWorldPosition(originalPosition);
	SetWorldRotation(originalRotation);
	SetWorldScale(originalScale);
}

std::shared_ptr<GameObject> GameObject::GetParent() const
{
    return Parent;
}

Vector3 GameObject::GetWorldPosition() const
{
    if (Parent != nullptr)
    {
        Vector3 v = Parent->GetWorldPosition();
        Debug::Log("A");
        Debug::Log(v);
        return v + Parent->GetWorldRotation().RotateVector(LocalPosition);
    }
    else
    {
        return LocalPosition;
    }
}

Quaternion GameObject::GetWorldRotation() const
{
    if (Parent != nullptr)
    {
        return Parent->GetWorldRotation() * LocalRotation;
    }
    else
    {
        return LocalRotation;
    }
}

Vector3 GameObject::GetWorldScale() const
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

bool GameObject::IsActiveInScene() const
{
    if (!IsActive)
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

void GameObject::SetWorldPosition(const Vector3& position)
{
    if (Parent != nullptr)
    {
        Vector3 parentWorldPosition = Parent->GetWorldPosition();
        Vector3 diff = position - parentWorldPosition;
        LocalPosition = diff;
    }
    else
    {
        LocalPosition = position;
    }
}

void GameObject::SetWorldRotation(const Quaternion& rotation)
{
    if (Parent != nullptr)
    {
        Quaternion parentWorldRotation = Parent->GetWorldRotation();
        Quaternion diff = parentWorldRotation.Inverse() * rotation;
        LocalRotation = diff;
    }
    else
    {
        LocalRotation = rotation;
    }
}

void GameObject::SetWorldScale(const Vector3& scale)
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

void GameObject::Start_ENGINE()
{
    for (auto& component : Components)
    {
        component->Start_ENGINE();
    }
}

void GameObject::Update_ENGINE()
{
    for (auto& component : Components)
    {
        component->Update_ENGINE();
    }
}

void GameObject::LateUpdate_ENGINE()
{
    for (auto& component : Components)
    {
        component->LateUpdate_ENGINE();
    }
}
