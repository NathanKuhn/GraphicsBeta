#include "ChunkMesh.hpp"
#include <iostream>

namespace Renderer {

	ChunkMesh::ChunkMesh(const Data::Chunk& chunkData, const Data::World& worldData, const Renderer::TextureAtlas& textureAtlas) {
		renderObject = RenderObject();
		renderObject.setPosition(chunkData.getChunkPosition() * CHUNK_SIZE);
		updateMesh(chunkData, worldData, textureAtlas);
	}

	void ChunkMesh::updateMesh(const Data::Chunk& chunkData, const Data::World& worldData, const Renderer::TextureAtlas& textureAtlas) {
		bool px, nx, py, ny, pz, nz;
		MeshData newMesh = MeshData();
		glm::ivec3 worldOffset = chunkData.getChunkPosition() * CHUNK_SIZE;
		for (int x = 0; x < CHUNK_SIZE; x++) {
			for (int y = 0; y < CHUNK_SIZE; y++) {
				for (int z = 0; z < CHUNK_SIZE; z++) {
					if (chunkData.getBlock(x, y, z)) {

						px = (x + 1 < CHUNK_SIZE) ? !chunkData.getBlock(x + 1, y, z) : !worldData.getBlock(glm::ivec3(x + 1, y, z) + worldOffset);
						nx = (x > 0) ? !chunkData.getBlock(x - 1, y, z) : !worldData.getBlock(glm::ivec3(x - 1, y, z) + worldOffset);

						py = (y + 1 < CHUNK_SIZE) ? !chunkData.getBlock(x, y + 1, z) : !worldData.getBlock(glm::ivec3(x, y + 1, z) + worldOffset);
						ny = (y > 0) ? !chunkData.getBlock(x, y - 1, z) : !worldData.getBlock(glm::ivec3(x, y - 1, z) + worldOffset);

						pz = (z + 1 < CHUNK_SIZE) ? !chunkData.getBlock(x, y, z + 1) : !worldData.getBlock(glm::ivec3(x, y, z + 1) + worldOffset);
						nz = (z > 0) ? !chunkData.getBlock(x, y, z - 1) : !worldData.getBlock(glm::ivec3(x, y, z - 1) + worldOffset);

						glm::vec3 offset(x, y, z);
						glm::vec2 uvOffset = textureAtlas.getOffset(chunkData.getBlock(x, y, z));
						float uvScale = textureAtlas.getScale();
						if (px) {
							_addMesh(newMesh, _POS_X_FACE, offset, uvOffset, uvScale);
						}
						if (nx) {
							_addMesh(newMesh, _NEG_X_FACE, offset, uvOffset, uvScale);
						}
						if (py) {
							_addMesh(newMesh, _POS_Y_FACE, offset, uvOffset, uvScale);
						}
						if (ny) {
							_addMesh(newMesh, _NEG_Y_FACE, offset, uvOffset, uvScale);
						}
						if (pz) {
							_addMesh(newMesh, _POS_Z_FACE, offset, uvOffset, uvScale);
						}
						if (nz) { 
							_addMesh(newMesh, _NEG_Z_FACE, offset, uvOffset, uvScale);
						}
					}
				}
			}
		}

		renderObject.updateMeshData(newMesh);
	}

	RenderObject ChunkMesh::getRenderObject() const {
		return renderObject;
	}

	void ChunkMesh::_addMesh(MeshData& destination, const MeshData& source, const glm::vec3 offset, const glm::vec2 uvOffset, const float uvScale) {
		int indexOffset = destination.vertices.size();

		for (int i = 0; i < source.vertices.size(); i++) {
			destination.vertices.push_back(source.vertices.at(i) + offset);
		}
	
		destination.normals.insert(destination.normals.end(), source.normals.begin(), source.normals.end());

		for (int i = 0; i < source.uvs.size(); i++) {
			destination.uvs.push_back(source.uvs.at(i) * uvScale + uvOffset);
		}

		for (int i = 0; i < source.triangles.size(); i++) {
			destination.triangles.push_back(source.triangles.at(i) + indexOffset);
		}
	}

}