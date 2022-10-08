#pragma once

#include "includes.hpp"
#include "logger.hpp"
#include "file.hpp"

#include <map>

namespace core {
	typedef unsigned int glShaderProgram;

	// A function which takes the specified files and
	// compiles/links those content to a shader program.
	glShaderProgram createShader(const char* vertexShaderFilename, const char* fragmentShaderFilename, const char* geometryShaderFilename = nullptr);
}

namespace engine {
	// A wrapper class to the glShaderProgram type
	// to extend it's functionality.
	class glShader {
	public:
		glShader(const char* vertexShaderFilename, const char* fragmentShaderFilename, const char* geometryShaderFilename = nullptr);
		~glShader();

		// Binds the program to be used in the next render function.
		void bind();
		// Sets a uniform in the shader.
		void setUniform(const char* name, bool value);
		void setUniform(const char* name, unsigned int value);
		void setUniform(const char* name, double value);
		void setUniform(const char* name, float value);
		void setUniform(const char* name, float x, float y);
		void setUniform(const char* name, float x, float y, float z);
		void setUniform(const char* name, float x, float y, float z, float w);
		void setUniform(const char* name, glm::vec2 vector);
		void setUniform(const char* name, glm::vec3 vector);
		void setUniform(const char* name, glm::vec4 vector);
		void setUniform(const char* name, glm::mat4x4 matrix);

	private:
		std::vector<std::pair<std::string, unsigned int>*> mUniformLocationCache;

		unsigned int mShaderIndex;

		unsigned int getUniformLocation(std::string name);
	};
}