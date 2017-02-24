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

void main() {
  vec3 lightDirection = normalize(spot.position - vertex_position);
  float distance = distance(spot.position, vertex_position);
  float attFactor = spot.constant + (spot.linear * distance) + (spot.quadratic * distance * distance);
  float intensity = pow(max(dot((-1 * spot.direction), lightDirection), 0.0f), spot.power);
  vec4 lightColour = spot.light_colour * (intensity / attFactor);
  vec3 viewDirection = normalize(eye_pos - vertex_position);

  float k1 = max(dot(transformed_normal, lightDirection), 0.0f);

  vec4 diffuse = k1 * (mat.diffuse_reflection * lightColour);

  vec3 H = normalize(lightDirection + viewDirection);
  float k2 = pow(max(dot(transformed_normal, H), 0.0f), mat.shininess);
  vec4 specular = k2 * (mat.specular_reflection * lightColour);

  vec4 texSample = texture(tex, tex_coord);
  vec4 primary = mat.emissive + diffuse;
  colour = primary * texSample + specular;
  colour.a = 1.0f;
}