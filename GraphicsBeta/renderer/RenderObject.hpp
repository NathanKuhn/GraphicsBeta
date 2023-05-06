#pragma once

#include <vector>

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "Mesh.hpp"

namespace Renderer {

	class RenderObject {

	private:
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
		RenderObject();
		RenderObject(Mesh mesh);

		/// <summary>
		/// Updates the graphics buffers with the mesh data.
		/// </summary>
		void UpdateMeshData(Mesh mesh);
		void cleanUp();

		glm::vec3 getPosition();
		glm::quat getRotation();
		glm::vec3 getEulerRotation();
		glm::vec3 getScale();
		glm::mat4 getTransform();
		GLuint getVAO();

		void setPosition(glm::vec3 newPosition);
		void setRotation(glm::quat newRotation);
		void setRotation(glm::vec3 eulerAngles);
		void setScale(glm::vec3 newScale);



	};

}

