#include "Chunk.hpp"

namespace Data {

	Chunk::Chunk(glm::ivec3 chunkPosition) :
		chunkPosition{ chunkPosition } {

		for (int block = 0; block < CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE; block++) {
			blockValues[block] = 0;
		}
	}

	Chunk::Chunk(const unsigned short* data, glm::ivec3 chunkPosition) :
		chunkPosition{ chunkPosition } {

		for (int block = 0; block < CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE; block++) {
			blockValues[block] = data[block];
		}
	}

	unsigned short Chunk::getBlock(glm::ivec3 subchunkPosition) const {
		return blockValues[subchunkPosition.x * CHUNK_SIZE * CHUNK_SIZE + subchunkPosition.y * CHUNK_SIZE + subchunkPosition.z];
	}

	unsigned short Chunk::getBlock(unsigned int x, unsigned int y, unsigned int z) const {
		return blockValues[x * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + z];
	}

	glm::ivec3 Chunk::getChunkPosition() const {
		return chunkPosition;
	}

	void Chunk::setBlock(glm::ivec3 subchunkPosition, unsigned short blockId) {
		blockValues[subchunkPosition.x * CHUNK_SIZE * CHUNK_SIZE + subchunkPosition.y * CHUNK_SIZE + subchunkPosition.x] = blockId;
	}
}