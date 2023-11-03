#include "Camera.hpp"

#include "glm/gtc/matrix_transform.hpp"

namespace Renderer {

	Camera::Camera(glm::vec3 pos, float fovy, float aspect) :
		position{ pos },
		rx{ 0 },
		ry{ 0 } {
		projection = glm::perspective(fovy, aspect, NEAR_PLANE, FAR_PLANE);
	}

	glm::mat4 Camera::getViewMatrix() const {
		glm::mat4 result = glm::mat4(1.0f);
		result = glm::rotate(result, rx, glm::vec3(1.0f, 0.0f, 0.0f));
		result = glm::rotate(result, ry, glm::vec3(0.0f, 1.0f, 0.0f));

		return glm::translate(result, -position);
	}

	glm::mat4 Camera::getRotationMatrix() const {
		glm::mat4 result = glm::mat4(1.0f);
		result = glm::rotate(result, rx, glm::vec3(1.0f, 0.0f, 0.0f));
		result = glm::rotate(result, ry, glm::vec3(0.0f, 1.0f, 0.0f));

		return result;
	}

	glm::mat4 Camera::getProjectionMatrix() const {
		return projection;
	}
}