#include "ChunkMesh.hpp"
#include <iostream>

namespace Renderer {

	ChunkMesh::ChunkMesh(const Data::Chunk& chunkData) {
		renderObject = RenderObject();
		renderObject.setPosition(chunkData.getChunkPosition() * CHUNK_SIZE);
		updateMesh(chunkData);
	}

	void ChunkMesh::updateMesh(const Data::Chunk& chunkData) {
		bool px, nx, py, ny, pz, nz;
		MeshData newMesh = MeshData();
		for (int x = 0; x < CHUNK_SIZE; x++) {
			for (int y = 0; y < CHUNK_SIZE; y++) {
				for (int z = 0; z < CHUNK_SIZE; z++) {
					if (chunkData.getBlock(x, y, z)) {
						px = x >= CHUNK_SIZE - 1 || !chunkData.getBlock(x + 1, y, z);
						nx = x <= 0 || !chunkData.getBlock(x - 1, y, z);

						py = y >= CHUNK_SIZE - 1 || !chunkData.getBlock(x, y + 1, z);
						ny = y <= 0 || !chunkData.getBlock(x, y - 1, z);

						pz = z >= CHUNK_SIZE - 1 || !chunkData.getBlock(x, y, z + 1);
						nz = z <= 0 || !chunkData.getBlock(x, y, z - 1);

						glm::vec3 offset(x, y, z);
						if (px) {
							_addMesh(newMesh, _POS_X_FACE, offset);
						}
						if (nx) {
							_addMesh(newMesh, _NEG_X_FACE, offset);
						}
						if (py) {
							_addMesh(newMesh, _POS_Y_FACE, offset);
						}
						if (ny) {
							_addMesh(newMesh, _NEG_Y_FACE, offset);
						}
						if (pz) {
							_addMesh(newMesh, _POS_Z_FACE, offset);
						}
						if (nz) { 
							_addMesh(newMesh, _NEG_Z_FACE, offset);
						}
					}
				}
			}
		}

		renderObject.updateMeshData(newMesh);
	}

	RenderObject ChunkMesh::getRenderObject() {
		return renderObject;
	}

	void ChunkMesh::_addMesh(MeshData& destination, const MeshData& source, const glm::vec3 offset) {
		int indexOffset = destination.vertices.size();

		destination.vertices.reserve(destination.vertices.size() + source.vertices.size());
		for (int i = 0; i < source.vertices.size(); i++) {
			destination.vertices.push_back(source.vertices.at(i) + offset);
		}
	
		destination.normals.insert(destination.normals.end(), source.normals.begin(), source.normals.end());
		destination.uvs.insert(destination.uvs.end(), source.uvs.begin(), source.uvs.end());
		
		destination.triangles.reserve(destination.triangles.size() + source.triangles.size());

		for (int i = 0; i < source.triangles.size(); i++) {
			destination.triangles.push_back(source.triangles.at(i) + indexOffset);
		}
	}

}