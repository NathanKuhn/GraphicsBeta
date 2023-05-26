#pragma once
#include <array>
#include "GL/glew.h"
#include "glm/glm.hpp"
#include "MeshData.hpp"
#include "RenderObject.hpp"
#include "../data/Chunk.hpp"
#include "../data/World.hpp"

namespace Renderer {
	class ChunkMesh {
		
	private:
		RenderObject renderObject;

		void _addMesh(MeshData& destination, const MeshData& source, const glm::vec3 offset);

	public:
		ChunkMesh(const Data::Chunk& chunkData, const Data::World& worldData);

		void updateMesh(const Data::Chunk& chunkData, const Data::World& worldData);
		RenderObject getRenderObject() const;
	};

    const MeshData _NEG_Z_FACE = MeshData(
        { glm::vec3(1, 0, 0), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), glm::vec3(1, 1, 0) },
        { glm::vec3(0, 0,-1), glm::vec3(0, 0,-1), glm::vec3(0, 0,-1), glm::vec3(0, 0,-1) },
        { glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1) },
        { glm::ivec3(0, 1, 2), glm::ivec3(0, 2, 3) } );

    const MeshData _POS_Z_FACE = MeshData(
        { glm::vec3(0, 0, 1), glm::vec3(1, 0, 1), glm::vec3(1, 1, 1), glm::vec3(0, 1, 1) },
        { glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 1) },
        { glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1) },
        { glm::ivec3(0, 1, 2), glm::ivec3(0, 2, 3) });

    const MeshData _POS_X_FACE = MeshData(
        { glm::vec3(1, 0, 1), glm::vec3(1, 0, 0), glm::vec3(1, 1, 0), glm::vec3(1, 1, 1) },
        { glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0), glm::vec3(1, 0, 0) },
        { glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1) },
        { glm::ivec3(0, 1, 2), glm::ivec3(0, 2, 3) });

    const MeshData _NEG_X_FACE = MeshData(
        { glm::vec3(0, 0, 0), glm::vec3(0, 0, 1), glm::vec3(0, 1, 1), glm::vec3(0, 1, 0) },
        { glm::vec3(-1, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(-1, 0, 0), glm::vec3(-1, 0, 0) },
        { glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1) },
        { glm::ivec3(0, 1, 2), glm::ivec3(0, 2, 3) });

    const MeshData _POS_Y_FACE = MeshData(
        { glm::vec3(0, 1, 1), glm::vec3(1, 1, 1), glm::vec3(1, 1, 0), glm::vec3(0, 1, 0) },
        { glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0), glm::vec3(0, 1, 0) },
        { glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1) },
        { glm::ivec3(0, 1, 2), glm::ivec3(0, 2, 3) });

    const MeshData _NEG_Y_FACE = MeshData(
        { glm::vec3(1, 0, 1), glm::vec3(0, 0, 1), glm::vec3(0, 0, 0), glm::vec3(1, 0, 0) },
        { glm::vec3(0,-1, 0), glm::vec3(0,-1, 0), glm::vec3(0,-1, 0), glm::vec3(0,-1, 0) },
        { glm::vec2(0, 0), glm::vec2(1, 0), glm::vec2(1, 1), glm::vec2(0, 1) },
        { glm::ivec3(0, 1, 2), glm::ivec3(0, 2, 3) });

}

