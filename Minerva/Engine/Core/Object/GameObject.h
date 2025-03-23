#pragma once

#include "Object.h"
#include "../../MMath/MMath.h"
#include "../Component/Component.h"
#include "../Scene/Scene.h"
#include  "../../Debug.h"

#include <memory>
#include <vector>

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
    GameObject();
    GameObject(Vector3 position, Quaternion rotation, Vector3 scale);

    static std::shared_ptr<GameObject> CreateGameObject(Scene& target);

    void SetActive(bool active);
    bool IsActiveInScene() const;

    void SetParent(std::shared_ptr<GameObject> parent);
    std::shared_ptr<GameObject> GetParent() const;

    Vector3    GetLocalPosition() const;
    Quaternion GetLocalRotation() const;
    Vector3    GetLocalScale() const;
    void SetLocalPosition(const Vector3& position);
    void SetLocalRotation(const Quaternion& rotation);
    void SetLocalScale(const Vector3& scale);

    Vector3    GetWorldPosition() const;
    Quaternion GetWorldRotation() const;
    Vector3    GetWorldScale() const;
    void SetWorldPosition(const Vector3& position);
    void SetWorldRotation(const Quaternion& rotation);
    void SetWorldScale(const Vector3& scale);


    template<typename T>
    std::shared_ptr<T> AddComponent()
    {
        std::shared_ptr<T> component = std::make_shared<T>();
        component.get()->Owner = shared_from_this();
        Components.push_back(component);
        return component;
    }

    void Start_ENGINE();
    void Update_ENGINE();
    void LateUpdate_ENGINE();
};
