#version 400 core

#define dot(x, y) max(dot(x, y), 0)

in vec3 frag_pos, normal;

out vec3 frag;

uniform vec3
	light_pos,
	light_color,
	eye_pos,
	obj_color;

uniform struct Material {
	float shineness;
} material;

void main() {

	vec3
		light_dir = normalize(light_pos - frag_pos),
		eye_dir = normalize(eye_pos - frag_pos),
		ref_dir = reflect(-light_dir, normal),
		ambient = light_color * .1,
		diffuse = light_color * dot(normal, light_dir),
		spec = light_color * pow(dot(ref_dir, eye_dir), material.shineness);

	frag = (ambient + diffuse + spec) * obj_color;
}
