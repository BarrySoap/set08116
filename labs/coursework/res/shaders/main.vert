#version 450

// Model transformation matrix
uniform mat4 M;
uniform mat4 MVP;
uniform mat4 MVPS;
uniform mat3 N;

layout(location = 0) in vec3 position;
layout(location = 2) in vec3 normal;
layout(location = 3) in vec3 binormal;
layout(location = 4) in vec3 tangent;
layout(location = 10) in vec2 tex_coord_in;

layout(location = 0) out vec3 vertex_position;
layout(location = 1) out vec3 transformed_normal;
layout(location = 2) out vec3 transformed_binormal;
layout(location = 3) out vec3 transformed_tangent;
layout(location = 4) out vec2 tex_coord_out;
layout(location = 5) out vec4 light_pos;

void main() {
  gl_Position = MVP * vec4(position, 1.0f);

  vertex_position = (M * vec4(position, 1.0f)).xyz;
  light_pos = MVPS * vec4(position, 1.0);
  transformed_normal = N * normal;
  transformed_binormal = normalize(N * binormal);
  transformed_tangent = normalize(N * tangent);
  tex_coord_out = tex_coord_in;
}