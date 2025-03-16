#pragma once
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "../../MMath/MMath.h"
#include "../../../Engine/Graphics3D/RenderData.h"

namespace MeshReader
{
	void ReadMesh(std::string fileName, RenderData* target)
	{
		std::ifstream file(fileName);
		if (!file.is_open()) {
			throw std::runtime_error("Failed to open file.");
		}
		int nV = 0; int nI = 0; std::vector<Vector3> vertices; std::vector<int> indices;
		std::string line;

		while (std::getline(file, line)) {
			std::istringstream iss(line);
			std::string prefix;
			iss >> prefix;

			if (prefix == "v") {  // 정점 (Vertex)
				Vector3 v;
				iss >> v.x >> v.y >> v.z;
				vertices.push_back(v);
				(nV)++;
			}
			else if (prefix == "f") {  // 면 (Face)
				std::vector<int> faceIndices;
				std::string vertexToken;

				while (iss >> vertexToken) {
					std::istringstream tokenStream(vertexToken);
					std::string indexStr;
					std::getline(tokenStream, indexStr, '/'); // '/'를 기준으로 첫 번째 값만 가져옴
					int index = std::stoi(indexStr) - 1; // 1-based index → 0-based index
					faceIndices.push_back(index);
				}

				// 삼각형으로 변환
				if (faceIndices.size() >= 3) {
					for (size_t i = 1; i < faceIndices.size() - 1; ++i) {
						indices.push_back(faceIndices[0]);
						indices.push_back(faceIndices[i]);
						indices.push_back(faceIndices[i + 1]);
						(nI) += 3;
					}
				}
			}
		}

		file.close();
		//(*target).Vertices = vertices.data();
		//(*target).Triangles = indices.data();
		(*target).VerticesLength = nV;
		(*target).TrianglesLength = nI;

		(*target).Vertices = new Vector3[nV];
		for (int i = 0; i < nV; i++)
		{
			(*target).Vertices[i] = vertices[i];
		}
		(*target).Triangles = new int[nI];
		for (int i = 0; i < nI; i++)
		{
			(*target).Triangles[i] = indices[i];
		}
		(*target).CalculateNormals();
	}
}