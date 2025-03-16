#pragma once

#include "../MMath/MMath.h"
#include "../Graphics2D/RenderTarget.h"

class Camera
{
public:     // default constructor
	Camera() : Position(Vector3(0, 0, 0)), Direction(Vector3(0, 0, 1)), FOV(60), Width(0), Height(0), Target(0, 0)
	{

	}
	Camera(Vector3  pos, Vector3 dir, float fov, int width, int height) : Position(pos.x, pos.y, pos.z), Direction(dir.x, dir.y, dir.z), FOV(fov),
		Width(width), Height(height), Target(width, height)
	{

	}
	Camera(const Camera& cam) : Position(cam.Position), Direction(cam.Direction), FOV(cam.FOV),
		Width(cam.Width), Height(cam.Height), Target(cam.Width, cam.Height)
	{

	}
	RenderTarget Target;
	Vector3 Position;
	Vector3 Direction;
	float FOV;
	int Width;
	int Height;
	bool IsMainGameCamera;

	Matrix4x4 CreateVMatrix()
	{
		Vector3 forward = Direction;
		Vector3 right = Vector3::Cross(forward, Vector3(0, 1, 0));
		Vector3 up = Vector3::Cross(right, forward);
		Vector3 t = Position * -1;
		Matrix4x4 m = Matrix4x4(
			right.x, right.y, right.z, Vector3::Dot(right, t),
			up.x, up.y, up.z, Vector3::Dot(up, t),
			forward.x, forward.y, forward.z, Vector3::Dot(forward, t),
			0, 0, 0, 1);
		return m;
	}

	Matrix4x4 CreateVPMatrix()
	{
		float distance = 1;
		float NearPlaneDistance = 1;
		float FarPlaneDistance = 100;
		float height = tan(FOV * Deg2Rad * 0.5f) * distance * 2;
		float width = height * ((float)Width / Height);
		float f1 = NearPlaneDistance / width;
		float f2 = NearPlaneDistance / height;
		float f3 = (FarPlaneDistance + NearPlaneDistance) / (FarPlaneDistance - NearPlaneDistance);
		float f4 = -2 * FarPlaneDistance * NearPlaneDistance / (FarPlaneDistance - NearPlaneDistance);

		Vector3 zAxis = Direction;
		Vector3 xAxis = Vector3::Cross(zAxis, Vector3(0, 1, 0));
		Vector3 yAxis = Vector3::Cross(xAxis, zAxis);
		Vector3 t = Position * -1;

		return Matrix4x4(
			f1 * xAxis.x, f1 * xAxis.y, f1 * xAxis.z, f1 * Vector3::Dot(xAxis, t),
			f2 * yAxis.x, f2 * yAxis.y, f2 * yAxis.z, f2 * Vector3::Dot(yAxis, t),
			f3 * zAxis.x, f3 * zAxis.y, f3 * zAxis.z, f3 * Vector3::Dot(zAxis, t) + f4,
			zAxis.x, zAxis.y, zAxis.z, Vector3::Dot(zAxis, t));
	}
};