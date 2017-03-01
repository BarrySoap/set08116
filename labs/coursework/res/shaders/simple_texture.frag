#version 440

struct spot_light {
  vec4 light_colour;
  vec3 position;
  vec3 direction;
  float constant;
  float linear;
  float quadratic;
  float power;
};

struct material {
  vec4 emissive;
  vec4 diffuse_reflection;
  vec4 specular_reflection;
  float shininess;
};

uniform spot_light spot;
uniform material mat;
uniform vec3 eye_pos;
uniform sampler2D tex;

layout(location = 0) in vec3 vertex_position;
layout(location = 1) in vec3 transformed_normal;
layout(location = 2) in vec2 tex_coord;

layout(location = 0) out vec4 colour;

vec4 calculate_spot(in spot_light spot, in material mat, in vec3 vertex_position, in vec3 transformed_normal, in vec3 view_dir, in vec4 tex_colour)
{
	vec3 lightDirection = normalize(spot.position - position);
	float dist = distance(spot.position,position);
	float attFactor = (spot.constant + spot.linear * dist + spot.quadratic * dist * dist);
	float intensity = pow(max(dot(-1*spot.direction, lightDirection), 0.0f), spot.power);
	vec4 light_colour = (intensity / attFactor) * spot.light_colour;
	vec4 diffuse = (mat.diffuse_reflection * light_colour) * max(dot(transformed_normal, lightDirection), 0.0);
	vec3 H = normalize(lightDirection + view_dir);
	vec4 specular = (mat.specular_reflection * light_colour) * pow(max(dot(transformed_normal, H), 0.0), mat.shininess);	
	vec4 colour = ((mat.emissive + diffuse) * tex_colour) + specular;
	colour.a = 1.0;

	return colour;
}