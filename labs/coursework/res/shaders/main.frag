#version 450

#ifndef DIRECTIONAL_LIGHT
#define DIRECTIONAL_LIGHT
struct directional_light {
  vec4 ambient_intensity;
  vec4 light_colour;
  vec3 light_dir;
};
#endif

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
vec4 calculate_direction(in directional_light direction, in material mat, in vec3 normal, in vec3 view_dir, in vec4 tex_colour);
vec4 calculate_spot (in spot_light spot, in material mat, in vec3 position, in vec3 normal, in vec3 view_dir, in vec4 tex_colour);
vec3 normalMap(in vec3 normal, in vec3 binormal, in vec3 tangent, in sampler2D normal_map, in vec2 tex_coord);
float calculate_fog(in float fog_coord, in vec4 fog_colour, in float fog_start, in float fog_end, in float fog_density, in int fog_type);

uniform sampler2D tex;
uniform sampler2D normal_map;
uniform vec3 eye_pos;
uniform point_light points[7];
uniform spot_light spot;
uniform directional_light direction;
uniform material mat;

uniform vec4 fog_colour;
uniform float fog_start;
uniform float fog_end;
uniform float fog_density;
uniform int fog_type;

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 transformed_normal;
layout(location = 2) in vec3 transformed_binormal;
layout(location = 3) in vec3 transformed_tangent;
layout(location = 4) in vec2 tex_coord;
layout(location = 5) in vec4 light_pos;
layout(location = 6) in vec4 CS_position;

layout(location = 0) out vec4 colour;

void main () {
	vec3 view_dir = normalize(eye_pos - vertex_position);
	vec4 tex_colour = texture(tex, tex_coord);
	vec3 maybeNormal = normalMap(transformed_normal, transformed_binormal, transformed_tangent, normal_map, tex_coord);
	colour = vec4(0.0f, 0.0f, 0.0f, 1.0f);
	for(int i = 0; i < 7; i++)
	{
		colour += calculate_point(points[i], mat, vertex_position, maybeNormal, view_dir, tex_colour);
	}
	colour += calculate_spot(spot, mat, vertex_position, maybeNormal, view_dir, tex_colour);
	colour += calculate_direction(direction, mat, maybeNormal, view_dir, tex_colour);
	colour.a = 1.0f;

	float fog_coord = abs(CS_position.z / CS_position.w);
	float fog_factor = calculate_fog(fog_coord, fog_colour, fog_start, fog_end, fog_density, fog_type);
	colour = mix(colour, fog_colour, fog_factor);
}