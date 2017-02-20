#version 440

// Point light information
struct point_light {
  vec4 light_colour;
  vec3 position;
  float constant;
  float linear;
  float quadratic;
};

// Material information
struct material {
  vec4 emissive;
  vec4 diffuse_reflection;
  vec4 specular_reflection;
  float shininess;
};

// Point light for the scene
uniform point_light point;
// Material for the object
uniform material mat;
// Eye position
uniform vec3 eye_pos;
// Texture
uniform sampler2D tex;

// Incoming position
layout(location = 0) in vec3 vertex_position;
// Incoming normal
layout(location = 1) in vec3 transformed_normal;
// Incoming texture coordinate
layout(location = 2) in vec2 tex_coord;

// Outgoing colour
layout(location = 0) out vec4 colour;

void main() {
  // *********************************
  // Get distance between point light and vertex
  float distance = distance(point.position, vertex_position);
  // Calculate attenuation factor
  float attFactor = point.constant + (point.linear * distance) + (point.quadratic * distance * distance);
  // Calculate light colour
  vec4 lightColour = point.light_colour / attFactor;
  // Calculate light dir
  vec3 light_dir = normalize(point.position - vertex_position);
  // Now use standard phong shading but using calculated light colour and direction
  // - note no ambient
  float k1 = max(dot(transformed_normal, light_dir), 0.0f);
  vec4 diffuse = k1 * (mat.diffuse_reflection * lightColour);
  vec3 viewDirection = normalize(eye_pos - vertex_position);
  vec3 H = normalize(light_dir + viewDirection);
  float k2 = pow(max(dot(transformed_normal, H), 0.0f), mat.shininess);
  vec4 specular = k2 * (mat.specular_reflection * lightColour);
  vec4 texSample = texture(tex, tex_coord);
  vec4 primary = mat.emissive + diffuse;
  colour = primary * texSample + specular;
  colour.a = 1.0f;
  // *********************************
}