#pragma once


#include <vector>
#include <assimp/Importer.hpp>
#include <math.h>
#include "mesh.h"

class SphereMesh : public Mesh
{
public:
	SphereMesh();
	~SphereMesh();
	Generate(int p_ring_count, int radial_segment_count, double radius)
};

SphereMesh::SphereMesh() {
	vertices.push_back(Vertex())

}

SphereMesh::~SphereMesh() {

}

SphereMesh::Generate(int p_ring_count, int radial_segment_count, double radius) {
	vertices = {};
	indices = {};

	// TODO: Need to finish this.

	for (int y = 0; y < p_ring_count - 1; y++)
	{
		double pitch = 3.14 / static_cast<double>(y);

		for (int x = 0; x < length; x++)
		{
			double yaw;
			vertices.push_back(
				Vertex{
					glm::vec3(cos(yaw), 0.0f, 0.0f),
					glm::vec3(0.0f, 0.0f, 0.0f),
					glm::vec2(0.0f, 0.0f),
				}
			)
		}
	}
}

