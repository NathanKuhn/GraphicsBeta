#include "World.hpp"
#include <iostream>

namespace Data {

	World::World(unsigned int sizeX, unsigned int sizeY, unsigned int sizeZ) :
		worldSize{glm::ivec3(sizeX, sizeY, sizeZ)} {

		storedChunks = std::vector<Chunk>();
		storedChunks.reserve(sizeX * sizeY * sizeZ);


		for (int x = 0; x < sizeX; x++) {
			for (int y = 0; y < sizeY; y++) {
				for (int z = 0; z < sizeZ; z++) {
					storedChunks.push_back(Data::Chunk(glm::ivec3(x, y, z)));
				}
			}
		}

		return;
	}

	const Chunk& World::getChunk(glm::ivec3 chunkLocation) const {
		if (chunkLocation.x < 0 || chunkLocation.x >= worldSize.x ||
			chunkLocation.y < 0 || chunkLocation.y >= worldSize.y ||
			chunkLocation.z < 0 || chunkLocation.z >= worldSize.z) {
			std::cerr << "ERROR: tried to access chunk out of world bounds.\n";
		}
		return storedChunks.at(chunkLocation.x * worldSize.y * worldSize.z + chunkLocation.y * worldSize.z + chunkLocation.z);
	}

	unsigned short World::getBlock(glm::ivec3 worldPosition) const{
		glm::ivec3 chunkLocation = _getChunkLocation(worldPosition);
		if (chunkLocation.x < 0 || chunkLocation.x >= worldSize.x ||
			chunkLocation.y < 0 || chunkLocation.y >= worldSize.y ||
			chunkLocation.z < 0 || chunkLocation.z >= worldSize.z) {
			return 1;
		}

		Chunk chunk = getChunk(chunkLocation);
		glm::ivec3 subchunkLocation = _getSubchunkLocation(worldPosition);
		return chunk.getBlock(subchunkLocation);
	}

	glm::ivec3 World::getWorldSize() const {
		return worldSize;
	}

	bool World::isInWorld(glm::ivec3 worldPosition) const{
		glm::ivec3 chunkLocation = _getChunkLocation(worldPosition);
		return (0 <= chunkLocation.x && chunkLocation.x < worldSize.x) &&
			(0 <= chunkLocation.y && chunkLocation.y < worldSize.y) &&
			(0 <= chunkLocation.z && chunkLocation.z < worldSize.z);
	}

	void World::setBlock(glm::ivec3 worldPosition, unsigned short blockId){
		Chunk chunk = getChunk(_getChunkLocation(worldPosition));
		chunk.setBlock(_getSubchunkLocation(worldPosition), blockId);
	}

	glm::ivec3 World::_getChunkLocation(glm::ivec3 worldPosition) const {
		return glm::ivec3(
			(worldPosition.x >= 0) ? worldPosition.x / CHUNK_SIZE : worldPosition.x / CHUNK_SIZE - 1,
			(worldPosition.y >= 0) ? worldPosition.y / CHUNK_SIZE : worldPosition.y / CHUNK_SIZE - 1,
			(worldPosition.z >= 0) ? worldPosition.z / CHUNK_SIZE : worldPosition.z / CHUNK_SIZE - 1
		);
	}

	glm::ivec3 World::_getSubchunkLocation(glm::ivec3 worldPosition) const {
		glm::ivec3 result = glm::ivec3(
			worldPosition.x % CHUNK_SIZE,
			worldPosition.y % CHUNK_SIZE,
			worldPosition.z % CHUNK_SIZE
		);
		return result;
	}

}