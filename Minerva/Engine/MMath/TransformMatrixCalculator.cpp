#include "TransformMatrixCalculator.h"
#include "Constants.h"

Matrix4x4 CreateTransformMatrix(const Vector3& pos, const Quaternion& rot, const Vector3& scale)
{
    Vector3 rotation = rot.ToEulerAngles();
    rotation = rotation * Deg2Rad;
    float cx = cos(rotation.x);
    float sx = sin(rotation.x);
    float cy = cos(rotation.y);
    float sy = sin(rotation.y);
    float cz = cos(rotation.z);
    float sz = sin(rotation.z);
    return Matrix4x4(
        scale.x * (cz * cy), scale.y * (-sz * cx + cz * sy * sx), scale.z * (sz * sx + cz * sy * cx), -pos.x,
        scale.x * (sz * cy), scale.y * (cz * cx + sz * sy * sx), scale.z * (-cz * sx + sz * sy * cx), -pos.y,
        scale.x * (-sy), scale.y * (cy * sx), scale.z * (cy * cx), -pos.z,
        0, 0, 0, 1);
}

Matrix4x4 CreateRotationMatrix(const Quaternion& rot)
{
    Vector3 rotation = rot.ToEulerAngles();
    rotation = rotation * Deg2Rad;
    float cx = cos(rotation.x);
    float sx = sin(rotation.x);
    float cy = cos(rotation.y);
    float sy = sin(rotation.y);
    float cz = cos(rotation.z);
    float sz = sin(rotation.z);
    return Matrix4x4(
        (cz * cy), (-sz * cx + cz * sy * sx), (sz * sx + cz * sy * cx), 0,
        (sz * cy), (cz * cx + sz * sy * sx), (-cz * sx + sz * sy * cx), 0,
        (-sy), (cy * sx), (cy * cx), 0,
        0, 0, 0, 1);
}
