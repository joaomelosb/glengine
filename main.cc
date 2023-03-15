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

static float dt() {

	static Uint64 last;
	Uint64 current = SDL_GetTicks64();

	if (!last)
		last = current;

	float d = (current - last) / 1000.;
	last = current;

	return d;

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

#define CUBE(x) \
	x, x + 1, x + 2, \
	x, x + 2, x + 3

	GLfloat vertex[][2][3] = {
		// front
        {{-1, 1, 1}, {0, 0, 1}},
        {{1, 1, 1}, {0, 0, 1}},
        {{1, -1, 1}, {0, 0, 1}},
        {{-1, -1, 1}, {0, 0, 1}},

        // top
        {{-1, 1, -1}, {0, 1, 0}},
        {{1, 1, -1}, {0, 1, 0}},
        {{1, 1, 1}, {0, 1, 0}},
        {{-1, 1, 1}, {0, 1, 0}},

        // back
        {{-1, 1, -1}, {0, 0, -1}},
        {{1, 1, -1}, {0, 0, -1}},
        {{1, -1, -1}, {0, 0, -1}},
        {{-1, -1, -1}, {0, 0, -1}},

        // bottom
        {{-1, -1, -1}, {0, -1, 0}},
        {{1, -1, -1}, {0, -1, 0}},
        {{1, -1, 1}, {0, -1, 0}},
        {{-1, -1, 1}, {0, -1, 0}},

        // left
        {{-1, 1, -1}, {-1, 0, 0}},
        {{-1, 1, 1}, {-1, 0, 0}},
        {{-1, -1, 1}, {-1, 0, 0}},
        {{-1, -1, -1}, {-1, 0, 0}},

        // right
        {{1, 1, -1}, {1, 0, 0}},
        {{1, 1, 1}, {1, 0, 0}},
        {{1, -1, 1}, {1, 0, 0}},
        {{1, -1, -1}, {1, 0, 0}}
	};

	GLubyte indices[] = {
		CUBE(0),
		CUBE(4),
		CUBE(8),
		CUBE(12),
		CUBE(16),
		CUBE(20)
	};

	glGenVertexArrays(1, &VAO);
	glBindVertexArray(VAO);

	glGenBuffers(2, buffers);

	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof vertex, vertex,
		GL_STATIC_DRAW);

	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof *vertex, 0);
	glEnableVertexAttribArray(0);

	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof *vertex,
		(void *)sizeof **vertex);
	glEnableVertexAttribArray(1);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof indices, indices,
		GL_STATIC_DRAW);

	glEnable(GL_DEPTH_TEST);

	int w, h;

	SDL_GetWindowSize(window, &w, &h);

	glm::vec3 eye {0, 0, 5};

	for (float elapsed = 0, d; (d = dt(), true); elapsed += d) {

		SDL_Event e;

		while (SDL_PollEvent(&e))
			switch (e.type) {
			case SDL_QUIT:
				return 0;
			case SDL_WINDOWEVENT:
				if (e.window.event == SDL_WINDOWEVENT_SIZE_CHANGED) {
					glViewport(0, 0, w = e.window.data1,
						h = e.window.data2);
					DEBUG("size change (%dx%d)", w, h)
				}
			}

		glClearColor(.2, .3, .4, 1);
		glClear(GL_COLOR_BUFFER_BIT |
				GL_DEPTH_BUFFER_BIT);

		program.setUniform("p", glm::perspective(glm::pi<float>() / 4,
			static_cast<float>(w) / h, 1.f, 100.f));

		program.setUniform("v", glm::lookAt(eye, glm::vec3(0, 0, 0),
			glm::vec3(0, 1, 0)));

		program.setUniform("m", glm::rotate(glm::mat4(1), elapsed,
			glm::vec3(1, 1, 0)));

		program.setUniform("light_pos", glm::vec3(0, 0, 10));

		program.setUniform("light_color", glm::vec3(1));

		program.setUniform("eye_pos", eye);

		program.setUniform("obj_color", glm::vec3(.5));

		program.setUniform("material.shineness", 32);

		glDrawElements(GL_TRIANGLES, sizeof indices / sizeof *indices,
			GL_UNSIGNED_BYTE, 0);

		SDL_GL_SwapWindow(window);

	}

	return 0;
}
