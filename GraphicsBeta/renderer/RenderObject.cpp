#include "RenderObject.hpp"
#include <iostream>
#include "glm/gtc/matrix_transform.hpp"

namespace Renderer {

	RenderObject::RenderObject(Mesh mesh) : 
		mesh{ mesh } {

		position = glm::vec3(0.0f, 0.0f, 0.0f);
		rotation = glm::quat();
		scale = glm::vec3(1.0f, 1.0f, 1.0f);
		transform = glm::mat4(1.0f);

		/* Likely a better way to put the data into the buffers. */
		size_t vertDataSize = mesh.vertices.size() * 3 * sizeof(GLfloat);
		size_t indDataSize = mesh.triangles.size() * 3 * sizeof(GLuint);

		if (vertDataSize == 0 || indDataSize == 0) {
			std::cerr << "WARNING: Mesh with no triangles detected.\n";
			return;
		}

		GLfloat* posData = (GLfloat*)malloc(vertDataSize);
		GLfloat* normData = (GLfloat*)malloc(vertDataSize);
		GLuint* indData = (GLuint*)malloc(indDataSize);

		if (posData == NULL || normData == NULL || indData == NULL) {
			std::cerr << "Could not allocate memory for mesh data on heap.\n";
			exit(EXIT_FAILURE);
		}

		glm::vec3 pos;
		glm::vec3 norm;
		for (int vert = 0; vert < mesh.vertices.size(); vert++) {
			pos = mesh.vertices.at(vert);
			norm = mesh.normals.at(vert);

			posData[vert * 3] = pos.x;
			posData[vert * 3 + 1] = pos.y;
			posData[vert * 3 + 2] = pos.z;

			normData[vert * 3] = norm.x;
			normData[vert * 3 + 1] = norm.y;
			normData[vert * 3 + 2] = norm.z;
		}

		glm::ivec3 triangle;
		for (int triNum = 0; triNum < mesh.triangles.size(); triNum++) {
			triangle = mesh.triangles.at(triNum);

			indData[triNum * 3] = triangle.x;
			indData[triNum * 3 + 1] = triangle.y;
			indData[triNum * 3 + 2] = triangle.z;
		}

		glGenVertexArrays(1, &vao);
		glBindVertexArray(vao);

		/* Create position buffer and populate with data. */
		glGenBuffers(1, &vboPosition);
		glBindBuffer(GL_ARRAY_BUFFER, vboPosition);
		glBufferData(GL_ARRAY_BUFFER, vertDataSize, posData, GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		/* Create normal buffer and populate with data. */
		glGenBuffers(1, &vboNormal);
		glBindBuffer(GL_ARRAY_BUFFER, vboNormal);
		glBufferData(GL_ARRAY_BUFFER, vertDataSize, normData, GL_STATIC_DRAW);

		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

		/* Create index buffer and populate with data. */
		glGenBuffers(1, &ibo);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, indDataSize, indData, GL_STATIC_DRAW);
	}

	glm::vec3 RenderObject::GetPosition() {
		return position;
	}

	glm::quat RenderObject::GetRotation() {
		return rotation;
	}

	glm::vec3 RenderObject::GetEulerRotation() {
		return glm::eulerAngles(rotation);
	}

	glm::vec3 RenderObject::GetScale() {
		return scale;
	}

	glm::mat4 RenderObject::GetTransform() {
		return transform;
	}

	GLuint RenderObject::GetVAO() {
		return vao;
	}

	void RenderObject::SetPosition(glm::vec3 newPosition) {
		position = newPosition;
		_calcTransform();
	}

	void RenderObject::SetRotation(glm::quat newRotation) {
		rotation = newRotation;
		_calcTransform();
	}

	void RenderObject::SetRotation(glm::vec3 eulerAngles) {
		rotation = glm::quat(eulerAngles);
	}

	void RenderObject::SetScale(glm::vec3 newScale) {
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