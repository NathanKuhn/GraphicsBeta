#pragma once

#include "../data/World.hpp"
#include "ChunkMesh.hpp"
#include "ShaderProgram.hpp"
#include "Camera.hpp"

namespace Renderer {

	class WorldRenderer {

	private:
		const Data::World& world;
		const Camera& camera;
		ShaderProgram shader;
        ShaderProgram skyShader;
        RenderObject skyObject;
		std::vector<ChunkMesh> chunkMeshes;
		
	public:
		WorldRenderer(const Data::World& worldRef, const Camera& cameraRef);

		void Render() const;

		void cleanup() const;

        
	};

    const std::vector<glm::vec3> skyVerts = {
        glm::vec3(-1, -1, 0.99999),
        glm::vec3(1, -1, 0.99999),
        glm::vec3(1, 1, 0.99999),
        glm::vec3(-1, 1, 0.99999)
    };

    const std::vector<glm::vec3> skyNormals = {
        glm::vec3(0, 0, -1),
        glm::vec3(0, 0, -1),
        glm::vec3(0, 0, -1),
        glm::vec3(0, 0, -1)
    };

    const std::vector<glm::vec2> skyUvs = {
        glm::vec2(0, 0),
        glm::vec2(1, 0),
        glm::vec2(1, 1),
        glm::vec2(0, 1)
    };

    const std::vector<glm::ivec3> skyTris = {
        glm::ivec3(0, 1, 2),
        glm::ivec3(0, 2, 3)
    };

}

