#include "ShaderProgram.hpp"
#include <iostream>
#include <fstream>
#include <vector>
#include "../utils/FileUtils.h"

namespace Renderer {

	ShaderProgram::ShaderProgram(const std::string& vertexShaderFileName, const std::string& fragmentShaderFileName) {
		programID = _loadProgram(vertexShaderFileName, fragmentShaderFileName);
		uniformIDsByName = std::map<std::string, GLuint>();
	}

	void ShaderProgram::Enable() {
		glUseProgram(programID);
	}

	bool ShaderProgram::AddUniform(const std::string& uniformName) {
		GLuint uniformHandle = glGetUniformLocation(programID, uniformName.c_str());
		if (uniformHandle == -1) {
			std::cerr << "Failed to locate uniform \'" << uniformName << "\'. Are you sure it exists?\n";
			return false;
		}
		uniformIDsByName[uniformName] = uniformHandle;
		return true;
	}

	bool ShaderProgram::SetUniform(const std::string& uniformName, const glm::mat4& matrix) {
		if (!uniformIDsByName.count(uniformName)) {
			return false;
		}
		GLuint uniformHandle = uniformIDsByName[uniformName];
		glUniformMatrix4fv(uniformHandle, 1, GL_FALSE, &matrix[0][0]);
		return true;
	}

	bool ShaderProgram::SetUniform(const std::string& uniformName, const glm::vec3& vector) {
		if (!uniformIDsByName.count(uniformName)) {
			return false;
		}
		GLuint uniformHandle = uniformIDsByName[uniformName];
		glUniform3f(uniformHandle, vector.x, vector.y, vector.z);
		return true;
	}

	GLuint ShaderProgram::_loadProgram(const std::string& vertexFilePath, const std::string& fragmentFilePath) {
		GLuint VertexShaderID = glCreateShader(GL_VERTEX_SHADER);
		GLuint FragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);

		GLint Result = GL_FALSE;
		int InfoLogLength;

		// Compile Vertex Shader
		std::cout << "[ShaderProgram]: Compiling vertex shader from \"" << vertexFilePath << "\"\n";
		std::string vertexSource = readFile(vertexFilePath.c_str());
		const char* VertexSourcePointer = vertexSource.c_str();
		glShaderSource(VertexShaderID, 1, &VertexSourcePointer, NULL);
		glCompileShader(VertexShaderID);

		// Check Vertex Shader
		glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(VertexShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> VertexShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(VertexShaderID, InfoLogLength, NULL, &VertexShaderErrorMessage[0]);
			std::cout << &VertexShaderErrorMessage[0] << "\n[ShaderProgram]: Failed to compile vertex shader.\n";
			return -1;
		}

		// Compile Fragment Shader
		std::cout << "[ShaderProgram]: Compiling fragment shader from \"" << fragmentFilePath << "\"\n";
		std::string fragmentSource = readFile(fragmentFilePath.c_str());
		const char* FragmentSourcePointer = fragmentSource.c_str();
		glShaderSource(FragmentShaderID, 1, &FragmentSourcePointer, NULL);
		glCompileShader(FragmentShaderID);

		// Check Fragment Shader
		glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &Result);
		glGetShaderiv(FragmentShaderID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> FragmentShaderErrorMessage(InfoLogLength + 1);
			glGetShaderInfoLog(FragmentShaderID, InfoLogLength, NULL, &FragmentShaderErrorMessage[0]);
			std::cout << &FragmentShaderErrorMessage[0] << "\n[ShaderProgram]: Failed to compile fragment shader.\n";
		}

		// Link the program
		printf("[ShaderProgram]: Linking program...\n");
		GLuint ProgramID = glCreateProgram();
		glAttachShader(ProgramID, VertexShaderID);
		glAttachShader(ProgramID, FragmentShaderID);
		glLinkProgram(ProgramID);

		// Check the program
		glGetProgramiv(ProgramID, GL_LINK_STATUS, &Result);
		glGetProgramiv(ProgramID, GL_INFO_LOG_LENGTH, &InfoLogLength);
		if (InfoLogLength > 0) {
			std::vector<char> ProgramErrorMessage(InfoLogLength + 1);
			glGetProgramInfoLog(ProgramID, InfoLogLength, NULL, &ProgramErrorMessage[0]);
			std::cout << &ProgramErrorMessage[0] << "\n[ShaderProgram]: Failed to link shader program.\n";
		}
		std::cout << "[ShaderProgram]: Shader loading completed!\n";

		glDetachShader(ProgramID, VertexShaderID);
		glDetachShader(ProgramID, FragmentShaderID);

		glDeleteShader(VertexShaderID);
		glDeleteShader(FragmentShaderID);

		return ProgramID;
	}

}