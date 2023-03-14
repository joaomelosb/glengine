#include "vertexarray.h"

namespace gl {

VertexArray::VertexArray() {

	glGenVertexArrays(1, &VAO);
	glGenBuffers(2, buffers);

}

VertexArray::~VertexArray() {

	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(2, buffers);

}

void VertexArray::buffer(void const *ptr, size_t ln) {

	glBindBuffer(GL_ARRAY_BUFFER, *buffers);
	glBufferData(GL_ARRAY_BUFFER, ln, ptr, GL_STATIC_DRAW);

}

template<typename T>
void VertexArray::indices(T const *ptr, size_t count) {

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[1]);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof (T),
		ptr, GL_STATIC_DRAW);
	indexLen = count;

}

} /* namespace gl */
