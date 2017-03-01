#version 450

#ifndef POINT_LIGHT
#define POINT_LIGHT
struct point_light{
	vec4 light_colour;
	vec3 position;
	float constant;
	float linear;
	float quadratic;
};
#endif

#ifndef SPOT_LIGHT
#define SPOT_LIGHT
struct spot_light{
	vec4 light_colour;
	vec3 position;
	vec3 direction;
	float constant;
	float linear;
	float quadratic;
	float power;
};
#endif

#ifndef MATERIAL
#define MATERIAL
struct material{
	vec4 emissive;
	vec4 diffuse_reflection;
	vec4 specular_reflection;
	float shininess;
};
#endif

vec4 calculate_point (in point_light point, in material mat, in vec3 position, in vec3 normal, in vec3 view_dir, in vec4 tex_colour);
vec4 calculate_spot (in spot_light spot, in material mat, in vec3 position, in vec3 normal, in vec3 view_dir, in vec4 tex_colour);

uniform sampler2D tex;
uniform vec3 eye_pos;
uniform point_light point;
uniform spot_light spot;
uniform material mat;

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 transformed_normal;
layout(location = 2) in vec2 tex_coord;

layout(location = 0) out vec4 colour;

void main () {
	vec3 view_dir = normalize(eye_pos - vertex_position);
	vec4 tex_colour = texture(tex, tex_coord);
	colour += calculate_point(point, mat, vertex_position, transformed_normal, view_dir, tex_colour);
	//colour += calculate_spot(spot, mat, vertex_position, transformed_normal, view_dir, tex_colour);
	colour = tex_colour;
}