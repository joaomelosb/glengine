#version 400 core

// Vertex attributes
in vec4 pos;
in vec3 norm;

// varying variables
out vec3 frag_pos, normal;

// uniforms
uniform mat4 p, v, m;

void main() {
	gl_Position = p * v * m * pos;
	frag_pos = (m * pos).xyz;
	normal = normalize((m * vec4(norm, 0)).xyz);
}
