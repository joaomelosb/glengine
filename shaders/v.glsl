#version 400 core

in vec4 pos;

uniform mat4 mat;

void main() {
	gl_Position = mat * pos;
}
