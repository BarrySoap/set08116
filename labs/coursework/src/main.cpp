#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

map<string, mesh> meshes;
geometry geom;
effect eff;
texture tex;
free_camera cam;
material mat;
double cursor_x = 0.0;
double cursor_y = 0.0;

bool initialise() {
	glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetCursorPos(renderer::get_window(), &cursor_x, &cursor_y);
	return true;
}

bool load_content() {
	meshes["plane"] = mesh(geometry_builder::create_plane(250, 400));

	// ***** Create Front Walls *****
	meshes["FrontWallA"] = mesh(geometry_builder::create_box(vec3(20.0f, 20.0f, 1.0f)));
	meshes["FrontWallB"] = mesh(geometry_builder::create_box(vec3(20.0f, 20.0f, 1.0f)));
	meshes["FrontWallAB"] = mesh(geometry_builder::create_box(vec3(10.0f, 10.0f, 1.0f)));
	// ***** Move and Scale *****
	meshes["FrontWallA"].get_transform().scale = vec3(5.0f);
	meshes["FrontWallA"].get_transform().position = vec3(75.0f, 50.0f, 197.5f);
	meshes["FrontWallB"].get_transform().scale = vec3(5.0f);
	meshes["FrontWallB"].get_transform().position = vec3(-75.0f, 50.0f, 197.5f);
	meshes["FrontWallAB"].get_transform().scale = vec3(5.0f);
	meshes["FrontWallAB"].get_transform().position = vec3(0.0f, 75.0f, 197.5f);
	//**********************************************************************************//

	// ***** Create Right Wall *****
	meshes["RightWall"] = mesh(geometry_builder::create_box(vec3(1.0f, 20.0f, 80.0f)));
	// ***** Move and Scale *****
	meshes["RightWall"].get_transform().scale = vec3(5.0f);
	meshes["RightWall"].get_transform().position = vec3(122.5f, 50.0f, 0.0f);
	//**********************************************************************************//

	// ***** Create Left Wall *****
	meshes["LeftWall"] = mesh(geometry_builder::create_box(vec3(1.0f, 20.0f, 80.0f)));
	// ***** Move and Scale *****
	meshes["LeftWall"].get_transform().scale = vec3(5.0f);
	meshes["LeftWall"].get_transform().position = vec3(-122.5f, 50.0f, 0.0f);
	//**********************************************************************************//

	// ***** Create Back Wall *****
	meshes["BackWall"] = mesh(geometry_builder::create_box(vec3(50.0f, 20.0f, 1.0f)));
	// ***** Move and Scale *****
	meshes["BackWall"].get_transform().scale = vec3(5.0f);
	meshes["BackWall"].get_transform().position = vec3(0.0f, 50.0f, -197.5f);
	//**********************************************************************************//

	// ***** Create Roof *****
	meshes["Roof"] = mesh(geometry_builder::create_box(vec3(50.0f, 1.0f, 80.0f)));
	// ***** Move and Scale *****
	meshes["Roof"].get_transform().scale = vec3(5.0f);
	meshes["Roof"].get_transform().position = vec3(0.0f, 100.0f, 0);
	//**********************************************************************************//

	tex = texture("textures/stone.jpg");

  // Load in shaders
  eff.add_shader("shaders/simple_texture.vert", GL_VERTEX_SHADER);
  eff.add_shader("shaders/simple_texture.frag", GL_FRAGMENT_SHADER);
  // Build effect
  eff.build();

  // Set camera properties
  cam.set_position(vec3(0.0f, 10.0f, 400.0f));
  cam.set_target(vec3(0.0f, 0.0f, 0.0f));
  auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
  cam.set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
  return true;
}


bool update(float delta_time) {
	static double ratio_width = quarter_pi<float>() / static_cast<float>(renderer::get_screen_width());
	static double ratio_height =
		(quarter_pi<float>() *
		(static_cast<float>(renderer::get_screen_height()) / static_cast<float>(renderer::get_screen_width()))) /
		static_cast<float>(renderer::get_screen_height());

	double current_x;
	double current_y;

	glfwGetCursorPos(renderer::get_window(), &current_x, &current_y);

	double delta_x = current_x - cursor_x;
	double delta_y = current_y - cursor_y;

	delta_x = delta_x * ratio_width;
	delta_y = delta_y * ratio_height;
	cam.rotate(delta_x, delta_y);

	vec3 movement;
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_W)) {
		movement += vec3(0.0f, 0.0f, 1.0f);
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_A)) {
		movement += vec3(-1.0f, 0.0f, 0.0f);
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_S)) {
		movement += vec3(0.0f, 0.0f, -1.0f);
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_D)) {
		movement += vec3(1.0f, 0.0f, 0.0f);
	}

	cam.move(movement);
	cam.update(delta_time);

	glfwSetCursorPos(renderer::get_window(), cursor_x, cursor_y);

	return true;
}

bool render() {
	for (auto &e : meshes) {
		auto m = e.second;
		// Bind effect
		renderer::bind(eff);
		// Create MVP matrix
		auto M = m.get_transform().get_transform_matrix();
		auto V = cam.get_view();
		auto P = cam.get_projection();
		auto MVP = P * V * M;
		// Set MVP matrix uniform
		glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));

		renderer::bind(tex, 0);
		glUniform1i(eff.get_uniform_location("tex"), 0);

		// Render geometry
		renderer::render(m);
	}
	return true;
}

void main() {
  // Create application
  app application("Graphics Coursework");
  // Set load content, update and render methods
  application.set_load_content(load_content);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}