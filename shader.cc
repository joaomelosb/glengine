#include "shader.h"
#include "debug.h"

namespace gl {

Shader::Shader(GLenum _type, char const *src) :
	shader(glCreateShader(_type)),
	type(_type) {

	glShaderSource(shader, 1, &src, 0);

	glCompileShader(shader);

	if (!getParam(GL_COMPILE_STATUS)) {
		char *log = getInfoLog();
		E_DEBUG("failed to compile %s shader: %s",
			(char const *)*this, log);
		delete[] log;
		glDeleteShader(shader);
		shader = 0;
	}

}

Shader::~Shader() {

	glDeleteShader(shader);

}

Shader::operator bool() {
	return shader;
}

Shader::operator char const*() {
	switch (type) {
	case GL_VERTEX_SHADER:
		return "vertex";
	case GL_FRAGMENT_SHADER:
		return "fragment";
	}
	return "unknown";
}

GLint Shader::getParam(GLenum param) {

	GLint data;

	glGetShaderiv(shader, param, &data);

	return data;

}

char *Shader::getInfoLog(GLint *length)
	DEFAULT_GET_LOG(Shader, shader, length)

} /* namespace gl */
