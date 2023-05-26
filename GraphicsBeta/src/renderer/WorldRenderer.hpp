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
		const ShaderProgram& shader;
		std::vector<ChunkMesh> chunkMeshes;
		
	public:
		WorldRenderer(const Data::World& worldRef, const Camera& cameraRef, const ShaderProgram& shaderRef);

		void Render() const;

		void cleanup() const;
	};

}

