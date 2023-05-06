#include "RenderObject.hpp"
#include <iostream>
#include "glm/gtc/matrix_transform.hpp"

namespace Renderer {

	RenderObject::RenderObject() :
		vao{ 0 },
		vboPosition{ 0 },
		vboNormal{ 0 },
		ibo{ 0 },
		position{ glm::vec3(0.0f, 0.0f, 0.0f) },
		rotation{ glm::quat() },
		scale{ glm::vec3(1.0f, 1.0f, 1.0f) },
		transform{ glm::mat4(1.0f) } {
	}

	RenderObject::RenderObject(Mesh mesh) : RenderObject() {
		UpdateMeshData(mesh);
	}

	void RenderObject::UpdateMeshData(Mesh mesh) {
		size_t vertDataSize = mesh.vertices.size() * 3 * sizeof(GLfloat);
		size_t indDataSize = mesh.triangles.size() * 3 * sizeof(GLuint);

		if (vertDataSize == 0 || indDataSize == 0) {
			std::cerr << "WARNING: Mesh with no triangles detected.\n";
			return;
		}

		if (mesh.vertices.size() != mesh.normals.size()) {
			std::cout << "ERROR: Invalid mesh data.\n";
			return;
		}

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		/* Create position buffer and populate with data. */
		glGenBuffers(1, &vboPosition);
		glBindBuffer(GL_ARRAY_BUFFER, vboPosition);
		glBufferData(GL_ARRAY_BUFFER, vertDataSize, &mesh.vertices[0], GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		/* Create normal buffer and populate with data. */
		glGenBuffers(1, &vboNormal);
		glBindBuffer(GL_ARRAY_BUFFER, vboNormal);
		glBufferData(GL_ARRAY_BUFFER, vertDataSize, &mesh.normals[0], GL_STATIC_DRAW);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		/* Create index buffer and populate with data. */
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indDataSize, &mesh.triangles[0], GL_STATIC_DRAW);
	}

	void RenderObject::cleanUp() {
		glDeleteBuffers(1, &vboPosition);
		glDeleteBuffers(1, &vboNormal);
		glDeleteBuffers(1, &ibo);
		glDeleteVertexArrays(1, &vao);
	}

	glm::vec3 RenderObject::getPosition() {
		return position;
	}

	glm::quat RenderObject::getRotation() {
		return rotation;
	}

	glm::vec3 RenderObject::getEulerRotation() {
		return glm::eulerAngles(rotation);
	}

	glm::vec3 RenderObject::getScale() {
		return scale;
	}

	glm::mat4 RenderObject::getTransform() {
		return transform;
	}

	GLuint RenderObject::getVAO() {
		return vao;
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