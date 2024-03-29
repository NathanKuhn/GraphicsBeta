#include "WorldRenderer.hpp"


namespace Renderer {

	WorldRenderer::WorldRenderer(const Data::World& worldRef, const Camera& cameraRef, const Renderer::TextureAtlas& textureAtlas) :
		world{ worldRef },
		camera{ cameraRef },
		shader{ Renderer::ShaderProgram("resources/shaders/ChunkVertex.vert", "resources/shaders/ChunkFragment.frag") },
		skyShader{ Renderer::ShaderProgram("resources/shaders/SkyVertex.vert", "resources/shaders/SkyFragment.frag") } {

		shader.addUniform("uTransform");
		skyShader.addUniform("uTransform");

		MeshData skyMesh = MeshData(_SKY_VERTS, _SKY_NORMALS, _SKY_UVS, _SKY_TRIS);
		skyObject = RenderObject(skyMesh);

		glm::ivec3 worldSize = world.getWorldSize();
		chunkMeshes.reserve(worldSize.x * worldSize.y * worldSize.z);

		for (int x = 0; x < worldSize.x; x++) {
			for (int y = 0; y < worldSize.y; y++) {
				for (int z = 0; z < worldSize.z; z++) {
					chunkMeshes.push_back(
						ChunkMesh(
							world.getChunk(glm::ivec3(x, y, z)),
							world,
							textureAtlas));
				}
			}
		}

	}

	void WorldRenderer::Render() const {
		glm::mat4 worldToScreen = camera.getProjectionMatrix() * camera.getViewMatrix();
		glm::mat4 transform;

		/* Render the sky quad. */
		skyShader.enable();

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);
		glEnableVertexAttribArray(2);

		skyShader.setUniform("uTransform", glm::inverse(camera.getProjectionMatrix() * camera.getRotationMatrix()));
		glBindVertexArray(skyObject.getVAO());
		glDrawElements(GL_TRIANGLES, skyObject.getNumTriangles() * 3, GL_UNSIGNED_INT, NULL);

		glDisableVertexAttribArray(0);
		glDisableVertexAttribArray(1);
		glDisableVertexAttribArray(2);

		/* Render the chunks of the world. */
		shader.enable();

		for (int i = 0; i < chunkMeshes.size(); i++) {
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);

			Renderer::RenderObject chunkObject = chunkMeshes.at(i).getRenderObject();
			transform = worldToScreen * chunkObject.getTransform();
			shader.setUniform("uTransform", transform);
			glBindVertexArray(chunkObject.getVAO());
			glDrawElements(GL_TRIANGLES, chunkObject.getNumTriangles() * 3, GL_UNSIGNED_INT, NULL);

			glDisableVertexAttribArray(0);
			glDisableVertexAttribArray(1);
			glDisableVertexAttribArray(2);
		}
	}

	void WorldRenderer::cleanup() const {
		for (int i = 0; i < chunkMeshes.size(); i++) {
			chunkMeshes.at(i).getRenderObject().cleanUp();
		}
	}

}