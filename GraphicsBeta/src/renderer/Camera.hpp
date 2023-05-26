#pragma once
#include "glm/glm.hpp"

#define NEAR_PLANE 0.01f
#define FAR_PLANE 1000.0f

namespace Renderer {

	class Camera {

	private:
		glm::mat4 projection;

	public:
		glm::vec3 position;
		float rx;
		float ry;

		Camera(glm::vec3 pos, float fovy, float aspect);

		glm::mat4 getViewMatrix() const;
		glm::mat4 getRotationMatrix() const;
		glm::mat4 getProjectionMatrix() const;
	};

}