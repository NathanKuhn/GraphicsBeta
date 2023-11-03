#pragma once
#include <vector>
#include "glm/glm.hpp"

#define CHUNK_SIZE 32

namespace Data {

	class Chunk {

	private:
		unsigned short *blockValues;
		const glm::ivec3 chunkPosition;

	public:
		Chunk(glm::ivec3 chunkPosition);
		Chunk(const unsigned short* data, glm::ivec3 chunkPosition);

		unsigned short getBlock(glm::ivec3 subchunkPosition) const;
		unsigned short getBlock(unsigned int x, unsigned int y, unsigned int z) const;
		glm::ivec3 getChunkPosition() const;

		void setBlock(glm::ivec3 subchunkPosition, unsigned short blockId);
	};

}