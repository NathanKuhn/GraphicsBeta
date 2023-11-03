#pragma once

#include <vector>

#include "glm/glm.hpp"

namespace Renderer {

	struct MeshData {

	public:
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;
		std::vector<glm::ivec3> triangles;

		MeshData() :
			vertices {std::vector<glm::vec3>()},
			normals{ std::vector<glm::vec3>() },
			uvs{ std::vector<glm::vec2>() },
			triangles{ std::vector<glm::ivec3>() } {}

		MeshData(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> uvs, std::vector<glm::ivec3> triangles) :
			vertices{ vertices },
			normals{ normals },
			uvs{ uvs },
			triangles{ triangles } {}

	};

}
