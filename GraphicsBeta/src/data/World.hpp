#pragma once
#include <vector>
#include "glm/glm.hpp"

#include "Chunk.hpp"

namespace Data {

	class World {

	private:
		const glm::ivec3 worldSize;
		std::vector<Chunk> storedChunks;

		glm::ivec3 _getChunkLocation(glm::ivec3 worldPosition) const;
		glm::ivec3 _getSubchunkLocation(glm::ivec3 worldPosition) const;

	public:
		World(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ);

		const Chunk& getChunk(glm::ivec3 chunkLocation) const;

		/// <summary>
		/// Returns the block ID at a certain position in the world. Will return 0 if position is outside of world.
		/// </summary>
		unsigned short getBlock(glm::ivec3 worldPosition) const;
		glm::ivec3 getWorldSize() const;

		bool isInWorld(glm::ivec3 worldPosition) const;

		void setBlock(glm::ivec3 worldPosition, unsigned short blockId);

	};

}