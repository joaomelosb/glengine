#ifndef SHADER_H
#define SHADER_H

#include <GL/glew.h>
#include "globject.h"

namespace gl {

struct Shader : GLObject {

	friend class Program;

	explicit Shader(GLenum type, char const *src);

	~Shader();

	operator bool();

	operator char const*();

	// inherited from GLObject
	GLint getParam(GLenum) override;

	char *getInfoLog(GLint *length = nullptr) override;

private:

	GLuint shader;
	GLenum type;

};

} /* namespace gl */

#endif /* SHADER_H */
