
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
	// calculate distance to light
	float dist = distance(spot.position, position);
	vec3 lightDirection = normalize(spot.position - position);
	// calculate the light attenuation factor
	float attFactor = (spot.constant + spot.linear * dist + spot.quadratic * dist * dist);
	// calculate the light intensity
	float intensity = max(dot(-1 * spot.direction, lightDirection), 0.0f);
	vec4 light_colour = (intensity / attFactor) * spot.light_colour;
	// calculate diffuse component
	vec4 diffuse = (mat.diffuse_reflection * light_colour) * max(dot(normal, lightDirection), 0.0);

	// calculate the half vector
	vec3 H = normalize(lightDirection + view_dir);
	// calculate specular component
	vec4 specular = (mat.specular_reflection * light_colour) * pow(max(dot(normal, H), 0.0f), mat.shininess);

	vec4 colour = ((mat.emissive + diffuse) * tex_colour) + specular;
	colour.a = 1.0;
	return colour;
}