#include <sstream>
#include <fstream>
#include <cstdlib>
#include "program.h"
#include "shader.h"
#include "debug.h"
#include "glm/gtc/type_ptr.hpp"

namespace gl {

Program::Program() :
	program(glCreateProgram()) {}

Program::~Program() {

	glDeleteProgram(program);

}

bool Program::attachShader(GLenum type, char const *src) {

	Shader sh {type, src};

	if (!sh)
		return false;

	glAttachShader(program, sh.shader);

	return true;

}

bool Program::attachShaderFile(GLenum type, char const *file) {

	std::ifstream fs {file};

	if (!fs) {
		fprintf(stderr, "%s: filed to open stream: ", file);
		perror("");
		return false;
	}

	std::string s;

	// call std::basic_string<char>& std::basic_string<char>::operator=(std::basic_string<char> const&)
	return attachShader(type, (s = (std::ostringstream {} << fs.rdbuf()).str()).c_str());

}

bool Program::link() {

	glLinkProgram(program);

	if (!getParam(GL_LINK_STATUS)) {
		char *log = getInfoLog();
		E_DEBUG("link program failed: %s", log);
		delete[] log;
		return false;
	}

	return true;

}

void Program::use() {

	glUseProgram(program);

}

// Uniforms
// Name is damn long
#define uloc(n) glGetUniformLocation(program, n)

void Program::setUniform(char const *name, glm::mat4 const& mat) {

	glUniformMatrix4fv(
		uloc(name),
		1,
		GL_FALSE,
		glm::value_ptr(mat)
	);

}

GLint Program::getParam(GLenum param) {

	GLint data;

	glGetProgramiv(program, param, &data);

	return data;

}

char *Program::getInfoLog(GLint *length)
	DEFAULT_GET_LOG(Program, program, length)

} /* namespace gl */
