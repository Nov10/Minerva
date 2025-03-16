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

    // ������
    RenderData() : Vertices(nullptr), Normals(nullptr), VerticesLength(0), Triangles(nullptr),  TrianglesLength(0) {}

    // �Ҹ���
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
        // ���� ������ ����
        delete[] Vertices;
        delete[] Triangles;

        // ������ü�� 8�� ���� ������
        VerticesLength = 8;
        Vertices = new Vector3[VerticesLength]{
            Vector3(-1, -1, -1), // 0: ���ϵ�
            Vector3(1, -1, -1), // 1: ���ϵ�
            Vector3(1,  1, -1), // 2: ����
            Vector3(-1,  1, -1), // 3: �»��
            Vector3(-1, -1,  1), // 4: ���Ͼ�
            Vector3(1, -1,  1), // 5: ���Ͼ�
            Vector3(1,  1,  1), // 6: ����
            Vector3(-1,  1,  1)  // 7: �»��
        };
        TrianglesLength = 36;
        Triangles = new int[TrianglesLength] {
            // Front(����):  z = +0.5
            4, 5, 6,
                4, 6, 7,

                // Right(������): x = +0.5
                5, 1, 2,
                5, 2, 6,

                // Back(�ĸ�): z = -0.5
                1, 0, 3,
                1, 3, 2,

                // Left(����): x = -0.5
                0, 4, 7,
                0, 7, 3,

                // Top(��): y = +0.5
                3, 7, 6,
                3, 6, 2,

                // Bottom(�Ʒ�): y = -0.5
                1, 5, 4,
                1, 4, 0
            };


        CalculateNormals();
    }
};

