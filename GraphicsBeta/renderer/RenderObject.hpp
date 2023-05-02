#pragma once

#include <vector>

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "Mesh.hpp"

namespace Renderer {

	class RenderObject {

	private:
		Mesh mesh;
		glm::vec3 position;
		glm::quat rotation;
		glm::vec3 scale;
		glm::mat4 transform;
		GLuint vao;
		GLuint vboPosition;
		GLuint vboNormal;
		GLuint ibo;

		void _calcTransform();

	public:
		RenderObject(Mesh mesh);

		glm::vec3 GetPosition();
		glm::quat GetRotation();
		glm::vec3 GetEulerRotation();
		glm::vec3 GetScale();
		glm::mat4 GetTransform();
		GLuint GetVAO();

		void SetPosition(glm::vec3 newPosition);
		void SetRotation(glm::quat newRotation);
		void SetRotation(glm::vec3 eulerAngles);
		void SetScale(glm::vec3 newScale);


	};

}

