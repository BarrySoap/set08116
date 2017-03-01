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

vec4 calculate_spot (in spot_light spot, in material mat, in vec3 position, in vec3 normal, in vec3 view_dir, in vec4 tex_colour) {
	float dist = distance(light.position, position);
	vec3 lightDirection = normalize(light.position - position);
	float attFactor = (spot.constant + spot.linear * dist + spot.quadratic * dist * dist);
	float intensity = max(dot(-1 * light.direction, lightDirection), 0.0f);
	vec4 light_colour = (light_intensity / attenuationFac) * spot.light_colour;
	vec4 diffuse = (mat.diffuse_reflection * light_colour) * max(dot(normal, light_dir), 0.0);

	vec3 H = normalize(lightDirection + view_dir);
	vec4 specular = (mat.specular_reflection * light_colour) * pow(max(dot(normal, H), 0.0f), mat.shininess);

	vec4 colour = ((mat.emissive + diffuse) * tex_colour) + specular;
	colour.a = 1.0;
	return colour;
}