#include "Chunk.hpp"

#include <random>
#include <iostream>
#include "glm/gtc/noise.hpp"

namespace Data {

	Chunk::Chunk(glm::ivec3 chunkPosition) :
		chunkPosition{ chunkPosition } {

		std::default_random_engine gen;
		std::uniform_real_distribution<float> dist = std::uniform_real_distribution<float>(0.0f, 1.0f);

		int terrainHeight;
		glm::ivec3 worldPosition = chunkPosition * CHUNK_SIZE;

		blockValues = (unsigned short*)malloc(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * sizeof(unsigned short));
		if (blockValues == NULL) {
			std::cout << "Error allocating memory for chunk.\n";
			throw std::bad_alloc();
		}

		for (int x = 0; x < CHUNK_SIZE; x++) {
			for (int z = 0; z < CHUNK_SIZE; z++) {
				terrainHeight = glm::simplex(glm::vec2(worldPosition.x + x, worldPosition.z + z) / 40.0f) * 5.0f + 5.0f + glm::simplex(glm::vec2(worldPosition.x + x, worldPosition.z + z) / 150.0f) * 28 + 28;

				for (int y = 0; y < CHUNK_SIZE; y++) {
					
					if (worldPosition.y + y < terrainHeight)
						blockValues[x * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + z] = 1;
					else
						blockValues[x * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + z] = 0;
				}
			}
		}
	}

	Chunk::Chunk(const unsigned short* data, glm::ivec3 chunkPosition) :
		chunkPosition{ chunkPosition } {

		blockValues = (unsigned short*)malloc(CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE * sizeof(unsigned short));
		if (blockValues == NULL) {
			std::cout << "Error allocating memory for chunk.\n";
			throw std::bad_alloc();
		}

		for (int block = 0; block < CHUNK_SIZE * CHUNK_SIZE * CHUNK_SIZE; block++) {
			blockValues[block] = (data[block]);
		}
	}

	unsigned short Chunk::getBlock(glm::ivec3 subchunkPosition) const {
		if (subchunkPosition.x >= CHUNK_SIZE || subchunkPosition.y >= CHUNK_SIZE || subchunkPosition.z >= CHUNK_SIZE) {
			return 0;
		}
		return blockValues[subchunkPosition.x * CHUNK_SIZE * CHUNK_SIZE + subchunkPosition.y * CHUNK_SIZE + subchunkPosition.z];
	}

	unsigned short Chunk::getBlock(unsigned int x, unsigned int y, unsigned int z) const {
		if (x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE) {
			return 0;
		}
		return blockValues[x * CHUNK_SIZE * CHUNK_SIZE + y * CHUNK_SIZE + z];
	}

	glm::ivec3 Chunk::getChunkPosition() const {
		return chunkPosition;
	}

	void Chunk::setBlock(glm::ivec3 subchunkPosition, unsigned short blockId) {
		if (subchunkPosition.x >= CHUNK_SIZE || subchunkPosition.y >= CHUNK_SIZE || subchunkPosition.z >= CHUNK_SIZE) {
			return;
		}
		blockValues[subchunkPosition.x * CHUNK_SIZE * CHUNK_SIZE + subchunkPosition.y * CHUNK_SIZE + subchunkPosition.x] = blockId;
	}
}