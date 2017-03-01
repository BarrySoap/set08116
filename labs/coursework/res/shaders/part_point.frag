
#ifndef POINT_LIGHT
#define POINT_LIGHT

struct point_light
{
	vec4 light_colour;
	vec3 position;
	float constant;
	float linear;
	float quadratic;
};
#endif

#ifndef MATERIAL
#define MATERIAL
struct material
{
	vec4 emissive;
	vec4 diffuse_reflection;
	vec4 specular_reflection;
	float shininess;
};
#endif

vec4 calculate_point(in point_light point, in material mat, in vec3 position, in vec3 normal, in vec3 view_dir, in vec4 tex_colour)
{
	float dist = distance(point.position, position);
	float attFactor = point.constant + (point.linear * dist) + (point.quadratic * dist * dist);
	vec4 light_colour = point.light_colour / attFactor;
	light_colour.a = 1.0f;
	vec3 lightDirection = normalize(point.position-position);

	vec4 diffuse = (mat.diffuse_reflection * light_colour) * max(dot(normal, lightDirection), 0.0f);
	vec3 H = normalize(lightDirection + view_dir);
	vec4 specular = (mat.specular_reflection * light_colour) * pow(max(dot(normal, H), 0.0f), mat.shininess);
	vec4 primary = mat.emissive + diffuse;
	vec4 colour = primary * tex_colour + specular;
	colour.a = 1.0;
	return colour;
}