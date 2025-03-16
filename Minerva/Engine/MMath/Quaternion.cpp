#include "Quaternion.h"
#include "math.h"
#include "Constants.h"
#include <string>
#include <sstream>

Quaternion::Quaternion() : w(1), x(0), y(0), z(0) {}

Quaternion::Quaternion(float w, float x, float y, float z) : w(w), x(x), y(y), z(z) {}

Quaternion Quaternion::operator*(const Quaternion& q) const {
    return Quaternion(
        w * q.w - x * q.x - y * q.y - z * q.z,
        w * q.x + x * q.w + y * q.z - z * q.y,
        w * q.y - x * q.z + y * q.w + z * q.x,
        w * q.z + x * q.y - y * q.x + z * q.w
    );
}

Quaternion Quaternion::conjugate() const {
    return Quaternion(w, -x, -y, -z);
}

float Quaternion::magnitude() const {
    return sqrt(w * w + x * x + y * y + z * z);
}

Quaternion Quaternion::normalized() const {
    float mag = 1.0f / magnitude();
    return Quaternion(w * mag, x * mag, y * mag, z * mag);
}

Quaternion Quaternion::FromEulerAngles(float roll, float pitch, float yaw) {
    roll = roll * Deg2Rad;
    pitch = pitch * Deg2Rad;
    yaw = yaw * Deg2Rad;
    float cy = cos(yaw * 0.5);
    float sy = sin(yaw * 0.5);
    float cp = cos(pitch * 0.5);
    float sp = sin(pitch * 0.5);
    float cr = cos(roll * 0.5);
    float sr = sin(roll * 0.5);

    return Quaternion(
        cr * cp * cy + sr * sp * sy,
        sr * cp * cy - cr * sp * sy,
        cr * sp * cy + sr * cp * sy,
        cr * cp * sy - sr * sp * cy
    );
}

Quaternion Quaternion::FromEulerAngles(const Vector3& v)
{
    return FromEulerAngles(v.x, v.y, v.z);
}

Vector3 Quaternion::ToEulerAngles() const {
    float sinp = sinp = 2 * (w * y - z * x);
    return Vector3(
        atan2(2 * (w * x + y * z), 1 - 2 * (x * x + y * y)),
        fabsf(sinp) >= 1 ? copysign(PI / 2, sinp) : asin(sinp),
        atan2(2 * (w * z + x * y), 1 - 2 * (y * y + z * z))
    ) * Rad2Deg;
}

Quaternion Quaternion::Lerp(const Quaternion& q1, const Quaternion& q2, float t) {
    return Quaternion(
        q1.w + t * (q2.w - q1.w),
        q1.x + t * (q2.x - q1.x),
        q1.y + t * (q2.y - q1.y),
        q1.z + t * (q2.z - q1.z)
    ).normalized();
}

std::string Quaternion::ToString()
{
    std::ostringstream oss;
    oss << "(" << x << ", " << y << ", " << z << ", " << w << ")";
    return oss.str();
}

Vector3 Quaternion::RotateVector(const Vector3& v) const {
    Quaternion vecQuat(v.x, v.y, v.z, 0);  // 벡터를 (x, y, z, 0) 형태의 쿼터니언으로 변환
    Quaternion rotatedQuat = (*this) * vecQuat * conjugate();  // q * v * q^-1 계산

    return Vector3(rotatedQuat.x, rotatedQuat.y, rotatedQuat.z);  // 회전된 벡터 반환
}