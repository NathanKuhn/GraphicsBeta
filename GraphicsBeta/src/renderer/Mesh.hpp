#pragma once

#include <vector>

#include "glm/glm.hpp"

namespace Renderer {

	class Mesh {

	public:
		std::vector<glm::vec3> vertices;
		std::vector<glm::vec3> normals;
		std::vector<glm::vec2> uvs;
		std::vector<glm::ivec3> triangles;

		Mesh(std::vector<glm::vec3> vertices, std::vector<glm::vec3> normals, std::vector<glm::vec2> uvs, std::vector<glm::ivec3> triangles) :
			vertices{ vertices },
			normals{ normals },
			uvs{ uvs },
			triangles{ triangles } {}

	};

}
