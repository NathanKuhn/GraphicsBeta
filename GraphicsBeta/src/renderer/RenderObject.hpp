#pragma once

#include <vector>

#include "GL/glew.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"

#include "MeshData.hpp"

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
		GLuint vboUv;
		GLuint ibo;
		unsigned int numTriangles;
		unsigned int numVerts;

		void _calcTransform();

	public:
		RenderObject();
		RenderObject(MeshData mesh);

		/// <summary>
		/// Updates the graphics buffers with the mesh data.
		/// </summary>
		void updateMeshData(MeshData mesh);
		void cleanUp();

		glm::vec3 getPosition();
		glm::quat getRotation();
		glm::vec3 getEulerRotation();
		glm::vec3 getScale();
		glm::mat4 getTransform();
		GLuint getVAO();
		unsigned int getNumTriangles();
		unsigned int getNumVerts();

		void setPosition(glm::vec3 newPosition);
		void setRotation(glm::quat newRotation);
		void setRotation(glm::vec3 eulerAngles);
		void setScale(glm::vec3 newScale);



	};

}

