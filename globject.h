#ifndef GLOBJECT_H
#define GLOBJECT_H

#include <GL/glew.h>

// Use this macro for shader and program methods
#define DEFAULT_GET_LOG(name, obj, length) { \
	GLint len = getParam(GL_INFO_LOG_LENGTH); \
	char *buffer = new char[len]; \
	glGet##name##InfoLog(obj, len, 0, buffer); \
	if (length) \
		*length = len; \
	return buffer; \
}

namespace gl {

// interface for objects where you can get
// params and log information.
struct GLObject {

	virtual ~GLObject() = default;

	// our pure virtual member functions
	virtual GLint getParam(GLenum) = 0;

	virtual char *getInfoLog(GLint *) = 0;

};

} /* namespace gl */

#endif /* GLOBJECT_H */
