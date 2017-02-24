#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

map<string, mesh> meshes;
geometry geom;
effect eff;
map<string, texture> texs;
free_camera cam;
material mat;
spot_light light;
double cursor_x = 0.0;
double cursor_y = 0.0;

bool initialise() {
	glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetCursorPos(renderer::get_window(), &cursor_x, &cursor_y);
	return true;
}

bool load_content() {
	// ***** Create Plane/Flooring *****
	meshes["Floor"] = mesh(geometry_builder::create_plane(250, 400));
	meshes["FloorB"] = mesh(geometry_builder::create_box(vec3(50.0f, 2.0f, 10.0f)));

	meshes["FloorB"].get_transform().scale = vec3(5.0f);
	meshes["FloorB"].get_transform().position = vec3(0.0f, 5.0f, -170.0f);
	//**********************************************************************************/

	// ***** Create Front Walls *****
	meshes["WallFrontA"] = mesh(geometry_builder::create_box(vec3(20.0f, 20.0f, 1.0f)));
	meshes["WallFrontB"] = mesh(geometry_builder::create_box(vec3(20.0f, 20.0f, 1.0f)));
	meshes["WallFrontAB"] = mesh(geometry_builder::create_box(vec3(10.0f, 10.0f, 1.0f)));
	// ***** Move and Scale *****
	meshes["WallFrontA"].get_transform().scale = vec3(5.0f);
	meshes["WallFrontA"].get_transform().position = vec3(75.0f, 50.0f, 197.5f);
	meshes["WallFrontB"].get_transform().scale = vec3(5.0f);
	meshes["WallFrontB"].get_transform().position = vec3(-75.0f, 50.0f, 197.5f);
	meshes["WallFrontAB"].get_transform().scale = vec3(5.0f);
	meshes["WallFrontAB"].get_transform().position = vec3(0.0f, 75.0f, 197.5f);
	//**********************************************************************************//

	// ***** Create Right Wall *****
	meshes["WallRight"] = mesh(geometry_builder::create_box(vec3(1.0f, 20.0f, 80.0f)));
	// ***** Move and Scale *****
	meshes["WallRight"].get_transform().scale = vec3(5.0f);
	meshes["WallRight"].get_transform().position = vec3(122.5f, 50.0f, 0.0f);
	//**********************************************************************************//

	// ***** Create Left Wall *****
	meshes["WallLeft"] = mesh(geometry_builder::create_box(vec3(1.0f, 20.0f, 80.0f)));
	// ***** Move and Scale *****
	meshes["WallLeft"].get_transform().scale = vec3(5.0f);
	meshes["WallLeft"].get_transform().position = vec3(-122.5f, 50.0f, 0.0f);
	//**********************************************************************************//

	// ***** Create Back Wall *****
	meshes["WallBack"] = mesh(geometry_builder::create_box(vec3(50.0f, 20.0f, 1.0f)));
	// ***** Move and Scale *****
	meshes["WallBack"].get_transform().scale = vec3(5.0f);
	meshes["WallBack"].get_transform().position = vec3(0.0f, 50.0f, -197.5f);
	//**********************************************************************************//

	// ***** Create Roof *****
	meshes["WallTop"] = mesh(geometry_builder::create_box(vec3(50.0f, 1.0f, 80.0f)));
	// ***** Move and Scale *****
	meshes["WallTop"].get_transform().scale = vec3(5.0f);
	meshes["WallTop"].get_transform().position = vec3(0.0f, 100.0f, 0);
	//**********************************************************************************//

	// ***** Create Roof Pillar *****
	meshes["Roof"] = mesh(geometry_builder::create_pyramid(vec3(50.0f, 20.0f, 80.0f)));
	// ***** Move and Scale *****
	meshes["Roof"].get_transform().scale = vec3(5.0f);
	meshes["Roof"].get_transform().position = vec3(0.0f, 152.5f, 0);
	//**********************************************************************************//

	// ***** Create Front Pillars *****
	meshes["PillarFront"] = mesh(geometry_builder::create_cylinder(40, 40, vec3(5.0f, 40.0f, 5.0f)));
	meshes["PillarFrontB"] = mesh(geometry_builder::create_cylinder(40, 40, vec3(5.0f, 40.0f, 5.0f)));
	// ***** Move and Scale *****
	meshes["PillarFront"].get_transform().scale = vec3(5.0f);
	meshes["PillarFront"].get_transform().position = vec3(137.5f, 100.0f, 190.0f);
	meshes["PillarFrontB"].get_transform().scale = vec3(5.0f);
	meshes["PillarFrontB"].get_transform().position = vec3(-137.5f, 100.0f, 190.0f);
	//**********************************************************************************//

	// ***** Create Back Pillars *****
	meshes["PillarBack"] = mesh(geometry_builder::create_cylinder(40, 40, vec3(5.0f, 40.0f, 5.0f)));
	meshes["PillarBackB"] = mesh(geometry_builder::create_cylinder(40, 40, vec3(5.0f, 40.0f, 5.0f)));
	// ***** Move and Scale *****
	meshes["PillarBack"].get_transform().scale = vec3(5.0f);
	meshes["PillarBack"].get_transform().position = vec3(137.5f, 100.0f, -190.0f);
	meshes["PillarBackB"].get_transform().scale = vec3(5.0f);
	meshes["PillarBackB"].get_transform().position = vec3(-137.5f, 100.0f, -190.0f);
	//**********************************************************************************//

	// ***** Create Stands *****
	meshes["Stand"] = mesh(geometry_builder::create_box(vec3(2.0f, 5.0f, 2.0f)));
	meshes["StandB"] = mesh(geometry_builder::create_box(vec3(2.0f, 5.0f, 2.0f)));
	meshes["StandC"] = mesh(geometry_builder::create_box(vec3(2.0f, 5.0f, 2.0f)));
	meshes["StandD"] = mesh(geometry_builder::create_box(vec3(2.0f, 5.0f, 2.0f)));
	// ***** Move and Scale *****
	meshes["Stand"].get_transform().scale = vec3(5.0f);
	meshes["Stand"].get_transform().position = vec3(70.0f, 13.0f, 80.0f);
	meshes["StandB"].get_transform().scale = vec3(5.0f);
	meshes["StandB"].get_transform().position = vec3(-70.0f, 13.0f, 80.0f);
	meshes["StandC"].get_transform().scale = vec3(5.0f);
	meshes["StandC"].get_transform().position = vec3(70.0f, 13.0f, -80.0f);
	meshes["StandD"].get_transform().scale = vec3(5.0f);
	meshes["StandD"].get_transform().position = vec3(-70.0f, 13.0f, -80.0f);
	//*************************************************************************//

	// ***** Create Torus' *****
	meshes["Torus"] = mesh(geometry_builder::create_torus(20, 20, 1.0f, 3.0f));
	meshes["TorusB"] = mesh(geometry_builder::create_torus(20, 20, 1.0f, 3.0f));
	meshes["TorusC"] = mesh(geometry_builder::create_torus(20, 20, 1.0f, 3.0f));
	meshes["TorusD"] = mesh(geometry_builder::create_torus(20, 20, 1.0f, 3.0f));
	// ***** Move and Scale *****
	meshes["Torus"].get_transform().scale = vec3(5.0f);
	meshes["Torus"].get_transform().position = vec3(70.0f, 20.0f, 80.0f);
	meshes["TorusB"].get_transform().scale = vec3(5.0f);
	meshes["TorusB"].get_transform().position = vec3(-70.0f, 20.0f, 80.0f);
	meshes["TorusC"].get_transform().scale = vec3(5.0f);
	meshes["TorusC"].get_transform().position = vec3(70.0f, 20.0f, -80.0f);
	meshes["TorusD"].get_transform().scale = vec3(5.0f);
	meshes["TorusD"].get_transform().position = vec3(-70.0f, 20.0f, -80.0f);
	//************************************************************************//

	// ***** Create Spheres *****
	meshes["Sphere"] = mesh(geometry_builder::create_sphere(20, 20));
	meshes["SphereB"] = mesh(geometry_builder::create_sphere(20, 20));
	meshes["SphereC"] = mesh(geometry_builder::create_sphere(20, 20));
	meshes["SphereD"] = mesh(geometry_builder::create_sphere(20, 20));
	// ***** Move and Scale *****
	meshes["Sphere"].get_transform().scale = vec3(5.0f);
	meshes["Sphere"].get_transform().position = vec3(70.0f, 30.0f, 80.0f);
	meshes["SphereB"].get_transform().scale = vec3(5.0f);
	meshes["SphereB"].get_transform().position = vec3(-70.0f, 30.0f, 80.0f);
	meshes["SphereC"].get_transform().scale = vec3(5.0f);
	meshes["SphereC"].get_transform().position = vec3(70.0f, 30.0f, -80.0f);
	meshes["SphereD"].get_transform().scale = vec3(5.0f);
	meshes["SphereD"].get_transform().position = vec3(-70.0f, 30.0f, -80.0f);
	//************************************************************************//

	mat.set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	mat.set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	mat.set_shininess(25.0f);
	mat.set_diffuse(vec4(1.0f, 0.0f, 0.0f, 1.0f));

	texs["Wall"]  = texture("textures/InsideWall.jpg", true, true);
	texs["Floor"] = texture("textures/Floor.jpg", true, true);
	texs["Roof"] = texture("textures/Roof.jpg", true, true);
	texs["Pillar"] = texture("textures/Pillar.jpg", true, true);
	texs["Stand"] = texture("textures/Pillar.jpg", true, true);
	texs["Torus"] = texture("textures/Torus.jpg", true, true);
	texs["Sphere"] = texture("textures/Sphere.jpg", true, true);

	light.set_position(vec3(-25.0f, 10.0f, -10.0f));
	light.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	light.set_direction(normalize(vec3(0.0f, 0.0f, -1.0f)));
	light.set_range(20.0f);
	light.set_power(1.0f);

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
	static float range = 70.0f;

	// ***** Rotate the torus' and spheres along the y axis *****
	meshes["Torus"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f) * delta_time);
	meshes["TorusB"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f) * delta_time);
	meshes["TorusC"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f) * delta_time);
	meshes["TorusD"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f) * delta_time);
	meshes["Sphere"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f) * delta_time);
	meshes["SphereB"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f) * delta_time);
	meshes["SphereC"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f) * delta_time);
	meshes["SphereD"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f) * delta_time);
	//**************************************************************************************//

	if (glfwGetKey(renderer::get_window(), GLFW_KEY_UP)) {
		light.move(vec3(0.0f, 0.0f, -20.0f) * delta_time);
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_LEFT)) {
		light.move(vec3(-20.0f, 0.0f, 0.0f) * delta_time);
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_DOWN)) {
		light.move(vec3(0.0f, 0.0f, 20.0f) * delta_time);
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_RIGHT)) {
		light.move(vec3(20.0f, 0.0f, 0.0f) * delta_time);
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_O)) {
		range *= 1 + 0.5f * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_P)) {
		range *= 1 - 0.5f * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_I)) {
		light.rotate(vec3(0.3f, 0.0f, 0.0f));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_K)) {
		light.rotate(vec3(-0.3f, 0.0f, 0.0f));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_J)) {
		light.rotate(vec3(0.0f, 0.3f, 0.0f));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_L)) {
		light.rotate(vec3(0.0f, -0.3f, 0.0f));
	}

	light.set_range(range);

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
	cam.rotate(delta_x, -delta_y);

	vec3 movement;
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_W)) {
		movement += vec3(0.0f, 0.0f, 5.0f);
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_A)) {
		movement += vec3(-5.0f, 0.0f, 0.0f);
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_S)) {
		movement += vec3(0.0f, 0.0f, -5.0f);
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_D)) {
		movement += vec3(5.0f, 0.0f, 0.0f);
	}

	cam.move(movement);
	cam.update(delta_time);

	glfwSetCursorPos(renderer::get_window(), cursor_x, cursor_y);

	return true;
}

texture BindingHelper(string name) {
	if (name.substr(0, 6).compare("Pillar") == 0) {
		return texs["Pillar"];
	}
	else if (name.substr(0, 4).compare("Wall") == 0) {
		return texs["Wall"];
	}
	else if (name.substr(0, 4).compare("Roof") == 0) {
		return texs["Roof"];
	}
	else if (name.substr(0, 5).compare("Floor") == 0) {
		return texs["Floor"];
	}
	else if (name.substr(0, 5).compare("Stand") == 0) {
		return texs["Stand"];
	}
	else if (name.substr(0, 5).compare("Torus") == 0) {
		return texs["Torus"];
	}
	else if (name.substr(0, 6).compare("Sphere") == 0) {
		return texs["Sphere"];
	}
	else {
		return texs[name];
	}
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
		glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
		glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(m.get_transform().get_normal_matrix()));
		renderer::bind(m.get_material(), "mat");
		renderer::bind(light, "spot");

		renderer::bind(BindingHelper(e.first), 0);

		glUniform1i(eff.get_uniform_location("tex"), 0);
		glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(cam.get_position()));

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
  application.set_initialise(initialise);
  application.set_update(update);
  application.set_render(render);
  // Run application
  application.run();
}