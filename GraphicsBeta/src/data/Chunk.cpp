#include "Chunk.hpp"

#include <random>
#include "glm/gtc/noise.hpp"

namespace Data {

	Chunk::Chunk(glm::ivec3 chunkPosition) :
		chunkPosition{ chunkPosition } {

		std::default_random_engine gen;
		std::uniform_real_distribution<float> dist = std::uniform_real_distribution<float>(0.0f, 1.0f);

		int terrainHeight;
		glm::ivec3 worldPosition;

		blockValues.reserve(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
		for (int x = 0; x < CHUNK_SIZE; x++) {
			for (int y = 0; y < CHUNK_SIZE; y++) {
				for (int z = 0; z < CHUNK_SIZE; z++) {
					worldPosition = chunkPosition * CHUNK_SIZE + glm::ivec3(x, y, z);

					/* TODO perhaps this should take a lambda for generating the terrain height? */
					terrainHeight = glm::perlin(glm::vec2(worldPosition.x, worldPosition.z) / 50.0f) * 15.0f + 16.0f;
					if (worldPosition.y < terrainHeight)
						blockValues.push_back(1);
					else
						blockValues.push_back(0);
				}
			}
		}
	}

	Chunk::Chunk(const unsigned short* data, glm::ivec3 chunkPosition) :
		chunkPosition{ chunkPosition } {

		blockValues.reserve(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE);
		for (int block = 0; block < CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE; block++) {
			blockValues.push_back(data[block]);
		}
	}

	unsigned short Chunk::getBlock(glm::ivec3 subchunkPosition) const {
		if (subchunkPosition.x >= CHUNK_SIZE || subchunkPosition.y >= CHUNK_SIZE || subchunkPosition.z >= CHUNK_SIZE) {
			return 0;
		}
		return blockValues.at(subchunkPosition.x * CHUNK_SIZE * CHUNK_SIZE + subchunkPosition.y * CHUNK_SIZE + subchunkPosition.z);
	}

	unsigned short Chunk::getBlock(unsigned int x, unsigned int y, unsigned int z) const {
		if (x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE) {
			return 0;
		}
		return blockValues.at(x * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + z);
	}

	glm::ivec3 Chunk::getChunkPosition() const {
		return chunkPosition;
	}

	void Chunk::setBlock(glm::ivec3 subchunkPosition, unsigned short blockId) {
		if (subchunkPosition.x >= CHUNK_SIZE || subchunkPosition.y >= CHUNK_SIZE || subchunkPosition.z >= CHUNK_SIZE) {
			return;
		}
		blockValues.at(subchunkPosition.x * CHUNK_SIZE * CHUNK_SIZE + subchunkPosition.y * CHUNK_SIZE + subchunkPosition.x) = blockId;
	}
}