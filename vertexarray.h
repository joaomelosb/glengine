#ifndef VERTEXARRAY_H
#define VERTEXARRAY_H

#include <GL/glew.h>

namespace gl {

struct VertexArray {

	VertexArray();

	~VertexArray();

	// fill VBO buffer
	void buffer(void const *, size_t);

	// fill EBO buffer
	template<typename T>
	void indices(T const *, size_t);

private:

	GLuint VAO, buffers[2];
	size_t indexLen;

};

} /* namespace gl */

#endif /* VERTEXARRAY_H */
