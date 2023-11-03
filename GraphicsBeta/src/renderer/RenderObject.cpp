#include "RenderObject.hpp"
#include <iostream>
#include "glm/gtc/matrix_transform.hpp"

namespace Renderer {

	RenderObject::RenderObject() :
		vao{ 0 },
		vboPosition{ 0 },
		vboNormal{ 0 },
		vboUv{ 0 },
		ibo{ 0 },
		numTriangles{ 0 },
		numVerts{ 0 },
		position{ glm::vec3(0.0f, 0.0f, 0.0f) },
		rotation{ glm::quat(0, 0, 0, 0) },
		scale{ glm::vec3(1.0f, 1.0f, 1.0f) },
		transform{ glm::mat4(1.0f) } {
	}

	RenderObject::RenderObject(MeshData mesh) : RenderObject() {
		updateMeshData(mesh);
	}

	void RenderObject::updateMeshData(MeshData mesh) {
		size_t vertDataSize = mesh.vertices.size() * 3 * sizeof(GLfloat);
		size_t uvDataSize = mesh.vertices.size() * 2 * sizeof(GLfloat);
		size_t indDataSize = mesh.triangles.size() * 3 * sizeof(GLuint);

		// TODO this need to not generate new buffers every time, but use the old ones and handle meshes of size 0.
		if (vertDataSize == 0) {
			return;
		}

		if (mesh.vertices.size() != mesh.normals.size()) {
			std::cout << "ERROR: Invalid mesh data.\n";
			return;
		}

		numTriangles = mesh.triangles.size();
		numVerts = mesh.vertices.size();

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		/* Create position buffer and populate with data. */
		glGenBuffers(1, &vboPosition);
		glBindBuffer(GL_ARRAY_BUFFER, vboPosition);
		glBufferData(GL_ARRAY_BUFFER, vertDataSize, &mesh.vertices.at(0), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		/* Create normal buffer and populate with data. */
		glGenBuffers(1, &vboNormal);
		glBindBuffer(GL_ARRAY_BUFFER, vboNormal);
		glBufferData(GL_ARRAY_BUFFER, vertDataSize, &mesh.normals.at(0), GL_STATIC_DRAW);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		/* Create uv buffer and populate with data. */
		glGenBuffers(1, &vboUv);
		glBindBuffer(GL_ARRAY_BUFFER, vboUv);
		glBufferData(GL_ARRAY_BUFFER, uvDataSize, &mesh.uvs.at(0), GL_STATIC_DRAW);

		glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, (void*)0);

		/* Create index buffer and populate with data. */
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indDataSize, &mesh.triangles.at(0), GL_STATIC_DRAW);
	}

	void RenderObject::cleanUp() {
		glDeleteBuffers(1, &vboPosition);
		glDeleteBuffers(1, &vboNormal);
		glDeleteBuffers(1, &ibo);
		glDeleteVertexArrays(1, &vao);
	}

	glm::vec3 RenderObject::getPosition() const {
		return position;
	}

	glm::quat RenderObject::getRotation() const {
		return rotation;
	}

	glm::vec3 RenderObject::getEulerRotation() const {
		return glm::eulerAngles(rotation);
	}

	glm::vec3 RenderObject::getScale() const {
		return scale;
	}

	glm::mat4 RenderObject::getTransform() const {
		return transform;
	}

	GLuint RenderObject::getVAO() const {
		return vao;
	}

	unsigned int RenderObject::getNumTriangles() const {
		return numTriangles;
	}

	unsigned int RenderObject::getNumVerts() const {
		return numVerts;
	}

	void RenderObject::setPosition(glm::vec3 newPosition) {
		position = newPosition;
		_calcTransform();
	}

	void RenderObject::setRotation(glm::quat newRotation) {
		rotation = newRotation;
		_calcTransform();
	}

	void RenderObject::setRotation(glm::vec3 eulerAngles) {
		rotation = glm::quat(eulerAngles);
	}

	void RenderObject::setScale(glm::vec3 newScale) {
		scale = newScale;
		_calcTransform();
	}

	void RenderObject::_calcTransform() {
		transform = glm::translate(
			glm::mat4(rotation) * glm::scale(glm::mat4(1.0f), scale),
			position
		);
	}

}