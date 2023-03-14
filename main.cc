#include <stdio.h>
#include <stdlib.h>
#include <GL/glew.h>
#define SDL_MAIN_HANDLED
#include <SDL2/SDL.h>
#include "program.h"
#include "debug.h"

// experimental
#include "glm/gtc/matrix_transform.hpp"

#define ASSERT(expr, ...) \
	if (!(expr)) { \
		fprintf(stderr, __VA_ARGS__); \
		fputc('\n', stderr); \
		return 1; \
	}

#define SDL_ASSERT(expr, s) ASSERT(expr, s ": %s", SDL_GetError())

static SDL_Window *window;
static SDL_GLContext context;

static void finish() {

	if (window) {
		DEBUG("destroy window")
		SDL_DestroyWindow(window);
	}

	if (context) {
		DEBUG("destroy context")
		SDL_GL_DeleteContext(context);
	}

	SDL_Quit();

}

int main() {

	atexit(finish);

	SDL_ASSERT(window = SDL_CreateWindow(
		"Window",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		500,
		500,
		SDL_WINDOW_RESIZABLE |
		SDL_WINDOW_OPENGL
	), "create window failed")

	SDL_ASSERT(context = SDL_GL_CreateContext(window),
		"create context failed")

	if (SDL_GL_SetSwapInterval(-1) && SDL_GL_SetSwapInterval(1))
		printf("warning: could not enable sync!\n");

	GLenum glew;

	ASSERT((glew = glewInit()) == GLEW_OK, "glew init failed: %s",
		glewGetErrorString(glew))

	gl::Program program;

	if (!program.attachShaderFile(GL_VERTEX_SHADER, "shaders/v.glsl") ||
		!program.attachShaderFile(GL_FRAGMENT_SHADER, "shaders/f.glsl") ||
		!program.link())
		return 1;

	program.use();

#define VBO *buffers
#define EBO buffers[1]
	GLuint VAO, buffers[2];

	GLfloat vertex[] = {
		-1, 1,
		1, 1,
		1, -1,
		-1, -1
	};

	GLubyte indices[] = {
		0, 1, 2,
		0, 2, 3
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(2, buffers);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertex, vertex,
		GL_STATIC_DRAW);

	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices,
		GL_STATIC_DRAW);

	for (;;) {

		SDL_Event e;

		while (SDL_PollEvent(&e))
			if (e.type == SDL_QUIT)
				return 0;

		glClearColor(.2, .3, .4, 1);
		glClear(GL_COLOR_BUFFER_BIT);

		glm::mat4 mat {1};

		program.setUniform("mat", mat);

		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, 0);

		SDL_GL_SwapWindow(window);

	}

	return 0;
}
