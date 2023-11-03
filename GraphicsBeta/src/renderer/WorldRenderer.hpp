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
		WorldRenderer(const Data::World& worldRef, const Camera& cameraRef, const Renderer::TextureAtlas& textureAtlas);

		void Render() const;

		void cleanup() const;

        
	};

    const std::vector<glm::vec3> _SKY_VERTS = {
        glm::vec3(-1, -1, 0.99999),
        glm::vec3(1, -1, 0.99999),
        glm::vec3(1, 1, 0.99999),
        glm::vec3(-1, 1, 0.99999)
    };

    const std::vector<glm::vec3> _SKY_NORMALS = {
        glm::vec3(0, 0, -1),
        glm::vec3(0, 0, -1),
        glm::vec3(0, 0, -1),
        glm::vec3(0, 0, -1)
    };

    const std::vector<glm::vec2> _SKY_UVS = {
        glm::vec2(0, 0),
        glm::vec2(1, 0),
        glm::vec2(1, 1),
        glm::vec2(0, 1)
    };

    const std::vector<glm::ivec3> _SKY_TRIS = {
        glm::ivec3(0, 1, 2),
        glm::ivec3(0, 2, 3)
    };

}

