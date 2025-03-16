#pragma once

#include "../MMath/MMath.h"

class RenderData
{
public:
	Vector3* Vertices;
    Vector3* Normals;
	int VerticesLength;

    int* Triangles;
    int TrianglesLength;

    // 생성자
    RenderData() : Vertices(nullptr), Normals(nullptr), VerticesLength(0), Triangles(nullptr),  TrianglesLength(0) {}

    // 소멸자
    ~RenderData() {
        delete[] Normals;
        delete[] Vertices;
        delete[] Triangles;
    }
    int GetVertices_Length() const
    {
        return VerticesLength;
    }
    const Vector3* const GetNormals() const
    {
        return Normals;
    }

    const Vector3* const GetVertices_LocalPosition() const
    {
        return Vertices;
    }

    int GetTriangles_Length() const
    {
        return TrianglesLength;
    }
    const int* const GetTriangles() const
    {
        return Triangles;
    }
    void CalculateNormals()
    {
        delete[] Normals;
        int vl = GetVertices_Length();
        int tl = GetTriangles_Length();

        Normals = new Vector3[vl];

        for (int i = 0; i < tl / 3; i++)
        {
            Vector3 p1 = Vertices[Triangles[3 * i]];
            Vector3 p2 = Vertices[Triangles[3 * i + 1]];
            Vector3 p3 = Vertices[Triangles[3 * i + 2]];

            Vector3 normal = Vector3::Cross(p2 - p1, p3 - p1);
            Normals[Triangles[3 * i]] = Normals[Triangles[3 * i]]+normal;
            Normals[Triangles[3 * i+1]] = Normals[Triangles[3 * i + 1]]+normal;
            Normals[Triangles[3 * i+2]] = Normals[Triangles[3 * i + 2]]+normal;
        }
        for (int i = 0; i < vl; i++)
        {
            Normals[i] = Normals[i].normalized();
        }
    }
    void AddCubeData() {
        // 기존 데이터 삭제
        delete[] Vertices;
        delete[] Triangles;

        // 정육면체의 8개 정점 데이터
        VerticesLength = 8;
        Vertices = new Vector3[VerticesLength]{
            Vector3(-1, -1, -1), // 0: 좌하뒤
            Vector3(1, -1, -1), // 1: 우하뒤
            Vector3(1,  1, -1), // 2: 우상뒤
            Vector3(-1,  1, -1), // 3: 좌상뒤
            Vector3(-1, -1,  1), // 4: 좌하앞
            Vector3(1, -1,  1), // 5: 우하앞
            Vector3(1,  1,  1), // 6: 우상앞
            Vector3(-1,  1,  1)  // 7: 좌상앞
        };
        TrianglesLength = 36;
        Triangles = new int[TrianglesLength] {
            // Front(정면):  z = +0.5
            4, 5, 6,
                4, 6, 7,

                // Right(오른쪽): x = +0.5
                5, 1, 2,
                5, 2, 6,

                // Back(후면): z = -0.5
                1, 0, 3,
                1, 3, 2,

                // Left(왼쪽): x = -0.5
                0, 4, 7,
                0, 7, 3,

                // Top(위): y = +0.5
                3, 7, 6,
                3, 6, 2,

                // Bottom(아래): y = -0.5
                1, 5, 4,
                1, 4, 0
            };


        CalculateNormals();
    }
};

