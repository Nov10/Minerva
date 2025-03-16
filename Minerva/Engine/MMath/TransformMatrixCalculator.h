#pragma once

#include "Vector3.h"
#include "Vector4.h"
#include "Quaternion.h"
#include "Matrix4x4.h"


Matrix4x4 CreateTransformMatrix(const Vector3& pos, const Quaternion& rot, const Vector3& scale);
Matrix4x4 CreateRotationMatrix(const Quaternion& rot);