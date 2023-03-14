#ifndef PROGRAM_H
#define PROGRAM_H

#include <GL/glew.h>
#include "glm/glm.hpp"
#include "globject.h"

namespace gl {

struct Program : GLObject {

	Program();

	~Program();

	bool attachShader(GLenum type, char const *src);

	bool attachShaderFile(GLenum type, char const *file);

	bool link();

	void use();

	// Handle uniforms
	void setUniform(char const *, glm::mat4 const&);

	GLint getParam(GLenum) override;

	char *getInfoLog(GLint *length = nullptr) override;

private:

	GLuint program;

};

} /* namespace gl */

#endif /* PROGRAM_H */
