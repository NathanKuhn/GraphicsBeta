#pragma once

#include <map>
#include <string>
#include "GL/glew.h"
#include "glm/glm.hpp"

namespace Renderer {

	class ShaderProgram {

	private:
		GLuint programID;
		std::map<std::string, GLuint> uniformIDsByName;

		static GLuint _loadProgram(const std::string& vertexFilePath, const std::string& fragmentFilePath);

	public:
		ShaderProgram(const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName);
		void enable() const;
		bool addUniform(const std::string& uniformName);
		bool setUniform(const std::string& uniformName, const glm::mat4& matrix) const;
		bool setUniform(const std::string& uniformName, const glm::vec3& vector) const;
	};

}
