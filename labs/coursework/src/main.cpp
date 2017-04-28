#include <glm\glm.hpp>
#include <graphics_framework.h>
#include "main.h"
#include "meshes.h"
#include "terrain.h"
#include "lightsAndMaterials.h"

using namespace std;
using namespace graphics_framework;
using namespace glm;

map<string, mesh> meshes;
map<mesh*, mesh*> transformed_hierarchy;
mesh skybox, terr;
geometry geom;
geometry screen_quad;
effect eff, greyscale_eff, sky_eff, terrain_eff, normals_eff, motion_blur, blur_eff;
map<string, texture> texs;
texture terrainTexs[4];
array<camera*, 2> cameras;
uint cameraType = 1;
uint targetCam = 1;
texture normalMap, blankNormal, modelTex;
material mat;
cubemap cube_map;
spot_light spotLight;
vector<point_light> pointLights(7);
directional_light directLight;
frame_buffer frames[2];
frame_buffer greyscale_frame;
frame_buffer temp_frame;
double cursor_x = 0.0;
double cursor_y = 0.0;
float temp = 0;
unsigned int current_frame = 0;
int fog = 2;

bool initialise() {
	// ***** Set up Free/Target Cameras *****
	cameras[0] = new target_camera();
	cameras[1] = new free_camera();
	/***************************************/

	// ***** Hide the cursor from the screen *****
	glfwSetInputMode(renderer::get_window(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	glfwGetCursorPos(renderer::get_window(), &cursor_x, &cursor_y);
	/**************************************************************************/

	return true;
}

bool load_content() {

	/***** Create Frame Buffers/Screen Quad *****/
	frames[0] = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());
	frames[1] = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());
	greyscale_frame = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());

	// Create a temp framebuffer
	temp_frame = frame_buffer(renderer::get_screen_width(), renderer::get_screen_height());

	vector<vec3> positions{ vec3(-1.0f, -1.0f, 0.0f), vec3(1.0f, -1.0f, 0.0f), vec3(-1.0f, 1.0f, 0.0f),
		vec3(1.0f, 1.0f, 0.0f) };
	vector<vec2> tex_coords{ vec2(0.0, 0.0), vec2(1.0f, 0.0f), vec2(0.0f, 1.0f), vec2(1.0f, 1.0f) };
	screen_quad.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
	screen_quad.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);
	screen_quad.set_type(GL_TRIANGLE_STRIP);
	/***************************************************************************************************/

	/***** Create All Meshes using meshes.cpp *****/
	Meshes();
	/**********************************************/

	/***** Terrain Generation *****/
	texture height_map("textures/heightmap.jpg");
	generate_terrain(geom, height_map, 8000, 8000, 900.0f);
	terr = mesh(geom);
	terr.get_transform().translate(vec3(1200.0f, -300.0f, 200.0f));
	/**************************************************************/

	// ***** Create Skybox Mesh *****
	skybox = mesh(geometry_builder::create_box(vec3(1.0f, 1.0f, 1.0f)));
	skybox.get_transform().scale *= vec3(100);
	/****************************************/

	// ***** Set Transform Hierarchies *****
	transformed_hierarchy[&meshes["TorusE"]] = &meshes["Torus"];
	transformed_hierarchy[&meshes["TorusF"]] = &meshes["TorusB"];
	transformed_hierarchy[&meshes["TorusG"]] = &meshes["TorusC"];
	transformed_hierarchy[&meshes["TorusH"]] = &meshes["TorusD"];
	/************************************************************/

	// ***** Move child Torus' so they're not in the same pos as parent
	meshes["TorusE"].get_transform().position = vec3(0.0f, 17.5f, 0.0f);
	meshes["TorusF"].get_transform().position = vec3(0.0f, 17.5f, 0.0f);
	meshes["TorusG"].get_transform().position = vec3(0.0f, 17.5f, 0.0f);
	meshes["TorusH"].get_transform().position = vec3(0.0f, 17.5f, 0.0f);
	/******************************************************************/

	// ***** Set Material Attributes using lightsAndMaterials.cpp *****
	SetMaterials();
	/************************************/

	// ***** Initialise Textures *****
	texs["Wall"]  = texture("textures/InsideWall.jpg", true, true);
	texs["Floor"] = texture("textures/Floor.jpg", true, true);
	texs["Roof"] = texture("textures/Roof.jpg", true, true);
	texs["Pillar"] = texture("textures/Pillar.jpg", true, true);
	texs["Stand"] = texture("textures/Pillar.jpg", true, true);
	texs["Torus"] = texture("textures/Torus.jpg", true, true);
	texs["Carpet"] = texture("textures/Carpet.jpg", true, true);
	texs["Cog"] = texture("textures/cog.jpg", true, true);
	texs["Drone"] = texture("textures/Drone.png", true, true);
	texs["Light"] = texture("textures/light.png", true, true);
	terrainTexs[0] = texture("textures/sand.jpg");
	terrainTexs[1] = texture("textures/grass.jpg");
	terrainTexs[2] = texture("textures/stone.jpg");
	terrainTexs[3] = texture("textures/snow.jpg");

	normalMap = texture("textures/RoofNormalMap.jpg", true, true);
	blankNormal = texture("textures/BlankNormal.jpg", true, true);
	array<string, 6> filenames = { "textures/emeraldfog_ft.tga", "textures/emeraldfog_bk.tga", "textures/emeraldfog_up.tga",
		"textures/emeraldfog_dn.tga", "textures/emeraldfog_rt.tga", "textures/emeraldfog_lf.tga" };
	/*************************************************************/

	// ***** Set Light Attributes using lightsAndMaterials.cpp *****
	SetLights();

	renderer::setClearColour(0.5f, 0.5f, 0.5f);
	/******************************************************/

	// ***** Create cube map *****
	cube_map = cubemap(filenames);
	/****************************/

  // ***** Load In Shaders *****
  eff.add_shader("shaders/main.vert", GL_VERTEX_SHADER);
  vector<string> frag_shaders{ "shaders/main.frag", "shaders/part_point.frag", "shaders/part_spot.frag", "shaders/part_normal.frag", 
							   "shaders/part_direction.frag", "shaders/part_fog.frag"};
  eff.add_shader(frag_shaders, GL_FRAGMENT_SHADER);
  eff.build();

  greyscale_eff.add_shader("shaders/simple.vert", GL_VERTEX_SHADER);
  greyscale_eff.add_shader("shaders/greyscale.frag", GL_FRAGMENT_SHADER);
  greyscale_eff.build();

  sky_eff.add_shader("shaders/skybox.vert", GL_VERTEX_SHADER);
  sky_eff.add_shader("shaders/skybox.frag", GL_FRAGMENT_SHADER);
  sky_eff.add_shader("shaders/part_direction.frag", GL_FRAGMENT_SHADER);
  sky_eff.add_shader("shaders/part_fog.frag", GL_FRAGMENT_SHADER);
  sky_eff.build();

  terrain_eff.add_shader("shaders/terrain.vert", GL_VERTEX_SHADER);
  terrain_eff.add_shader("shaders/terrain.frag", GL_FRAGMENT_SHADER);
  terrain_eff.add_shader("shaders/part_direction.frag", GL_FRAGMENT_SHADER);
  terrain_eff.add_shader("shaders/part_fog.frag", GL_FRAGMENT_SHADER);
  terrain_eff.add_shader("shaders/part_weighted_texture_4.frag", GL_FRAGMENT_SHADER);
  terrain_eff.build();

  normals_eff.add_shader("shaders/debug_shaders.vert", GL_VERTEX_SHADER);
  normals_eff.add_shader("shaders/debug_shaders.frag", GL_FRAGMENT_SHADER);
  normals_eff.add_shader("shaders/normals.geom", GL_GEOMETRY_SHADER);
  normals_eff.build();

  motion_blur.add_shader("shaders/simple.vert", GL_VERTEX_SHADER);
  motion_blur.add_shader("shaders/motion_blur.frag", GL_FRAGMENT_SHADER);
  motion_blur.build();

  blur_eff.add_shader("shaders/simple.vert", GL_VERTEX_SHADER);
  blur_eff.add_shader("shaders/blur.frag", GL_FRAGMENT_SHADER);
  blur_eff.build();
  /****************************************************************/

  // ***** Set Free Camera (Default) Properties *****
  cameras[1]->set_position(vec3(0.0f, 100.0f, 400.0f));
  cameras[1]->set_target(vec3(0.0f, 0.0f, 0.0f));
  cameras[1]->set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 5000.0f);
  auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
  /*****************************************************************************************************************/

  return true;
}


bool update(float delta_time) {

	current_frame = (current_frame + 1) % 2;

	/***** Fog Control *****/
	if (glfwGetKey(renderer::get_window(), '5')) {
		fog = 0;
	}
	if (glfwGetKey(renderer::get_window(), '6')) {
		fog = 1;
	}
	if (glfwGetKey(renderer::get_window(), '7')) {
		fog = 2;
	}
	/*********************************************/
	
	/***** Drone Movement *****/
	static float droneMovement = 1;
	vec3 rotation(rand() % 90 + 30);
	droneMovement += 0.1f * delta_time;
	
	meshes["DroneBase"].get_transform().translate(vec3(sinf(droneMovement), 0.0f, cos(droneMovement)) * 3.0f);
	meshes["DroneBase"].get_transform().rotate(rotation);

	meshes["DroneTop"].get_transform().position = meshes["DroneBase"].get_transform().position + vec3(0.0f, 23.0f, 0.0f);
	meshes["Light"].get_transform().position = meshes["DroneBase"].get_transform().position + vec3(0.0f, 28.0f, 0.0f);
	/******************************************************************************************************************/

	/***** FPS *****/
	cout << 1.0f / delta_time << endl;
	/********************************/

	// ***** Rotate the torus' along the y axis *****
	meshes["Torus"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f) * delta_time);
	meshes["TorusB"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f) * delta_time);
	meshes["TorusC"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f) * delta_time);
	meshes["TorusD"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f) * delta_time);
	meshes["TorusE"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f) * delta_time);
	meshes["TorusF"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f) * delta_time);
	meshes["TorusG"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f) * delta_time);
	meshes["TorusH"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f) * delta_time);
	//**************************************************************************************//

	/***** Cog Spinning *****/
	meshes["Cog"].get_transform().rotate(vec3(0.0f, -half_pi<float>(), 0.0f) * delta_time * 0.3f);

	temp += delta_time;
	if (temp < 1.0f) {
		meshes["Cog"].get_transform().rotate(vec3(0.0f, half_pi<float>(), 0.0f) * delta_time * 1.0f);
	}
	else if (temp > 2.0f) {
		temp = 0.0f;
	}
	/*****************************************************************************************/
	
	// ***** Movement for Spot Light *****
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_UP)) {
		spotLight.move(vec3(0.0f, 0.0f, -20.0f) * delta_time);
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_LEFT)) {
		spotLight.move(vec3(-20.0f, 0.0f, 0.0f) * delta_time);
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_DOWN)) {
		spotLight.move(vec3(0.0f, 0.0f, 20.0f) * delta_time);
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_RIGHT)) {
		spotLight.move(vec3(20.0f, 0.0f, 0.0f) * delta_time);
	}
	/*******************************************************/

	static float range = 70.0f;

	// ***** Spot Light Intensity *****
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_O)) {
		range *= 1 + 0.5f * delta_time;
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_P)) {
		range *= 1 - 0.5f * delta_time;
	}
	/***************************************************/

	// ***** Spot Light Rotations *****
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_I)) {
		spotLight.rotate(vec3(0.3f, 0.0f, 0.0f));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_K)) {
		spotLight.rotate(vec3(-0.3f, 0.0f, 0.0f));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_J)) {
		spotLight.rotate(vec3(0.0f, 0.3f, 0.0f));
	}
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_L)) {
		spotLight.rotate(vec3(0.0f, -0.3f, 0.0f));
	}
	/***************************************************/

	// ***** Set Light Ranges *****
	spotLight.set_range(range);
	for (int i = 0; i < 6; ++i) {
		pointLights[i].set_range(range);
	}
	/*****************************/

	// ***** Switch Back to Free Camera *****
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_LEFT_SHIFT))
	{
		cameraType = 1;
	}
	/**********************************************************/

	// ***** Switch to Target Camera 1 *****
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_1))
	{
		cameraType = 0;
		targetCam = 1;
	}
	// 2
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_2))
	{
		cameraType = 0;
		targetCam = 2;
	}
	// 3
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_3))
	{
		cameraType = 0;
		targetCam = 3;
	}
	// 4
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_4))
	{
		cameraType = 0;
		targetCam = 4;
	}
	/**************************************************/

	// ***** Target Camera Properties *****
	if (cameraType == 0)
	{
		switch (targetCam) {
		// If '1' is Pressed,
		case 1:
			cameras[0]->set_position(vec3(250.0f, 400.0f, -200.0f));
			cameras[0]->set_target(vec3(0.0f, 0.0f, 0.0f));
			break;
		// If '2' is Pressed,
		case 2:
			cameras[0]->set_position(vec3(-250.0f, 400.0f, -200.0f));
			cameras[0]->set_target(vec3(0.0f, 0.0f, 0.0f));
			break;
		// If '3' is Pressed,
		case 3:
			cameras[0]->set_position(vec3(250.0f, 400.0f, 200.0f));
			cameras[0]->set_target(vec3(0.0f, 0.0f, 0.0f));
			break;
		// If '4' is Pressed,
		case 4:
			cameras[0]->set_position(vec3(-250.0f, 400.0f, 200.0f));
			cameras[0]->set_target(vec3(0.0f, 0.0f, 0.0f));
			break;
		}
		cameras[0]->set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 2.414f, 1000.0f);
		cameras[0]->update(delta_time);
	}

	// ***** Free Camera Properties *****
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
	static_cast<free_camera*>(cameras[1])->rotate(delta_x, -delta_y);
	/***************************************************************/

	// ***** Free Camera Movement *****
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
	/***************************************************/

	static_cast<free_camera*>(cameras[1])->move(movement);
	static_cast<free_camera*>(cameras[1])->update(delta_time);

	glfwSetCursorPos(renderer::get_window(), cursor_x, cursor_y);
	skybox.get_transform().position = cameras[cameraType]->get_position();

	return true;
}

texture BindingHelper(string name) {
	// If the meshes name contains the string 'Pillar'
	if (name.substr(0, 6).compare("Pillar") == 0) {
		// return the texture name called "Pillar"
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
	else if (name.substr(0, 3).compare("Cog") == 0) {
		return texs["Cog"];
	}
	else if (name.substr(0, 5).compare("Drone") == 0) {
		return texs["Drone"];
	}
	else if (name.substr(0, 5).compare("Light") == 0) {
		return texs["Light"];
	}
	else {
		return texs[name];
	}
}

bool render() {

	/***** Frame Buffer Binding *****/
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_LEFT_ALT)) {
		renderer::set_render_target(temp_frame);
		// Clear frame
		renderer::clear();
	}
	/**********************************************************/

	/***** Greyscale *****/
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_RIGHT_CONTROL) || glfwGetKey(renderer::get_window(), GLFW_KEY_0)) {
		renderer::set_render_target(greyscale_frame);
		// Clear frame
		renderer::clear();
	}
	/**********************************************************/

	/***** V & P can be used across multiple effects *****/
	auto V = cameras[cameraType]->get_view();
	auto P = cameras[cameraType]->get_projection();
	auto PV = P * V;
	auto MVP = PV;
	/*****************************************************/

	/***** Showing Normals *****/
	if (glfwGetKey(renderer::get_window(), GLFW_KEY_LEFT_CONTROL)) {
		for (auto &e : meshes) {
			auto m = e.second;
			// Bind effect
			renderer::bind(normals_eff);
			// Create MVP matrix
			auto M = m.get_transform().get_transform_matrix();
			MVP = PV * M;
			// Set MVP matrix uniform
			glUniformMatrix4fv(normals_eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));

			// Render mesh
			renderer::render(m);
		}
	}
	/*****************************************************************************************/

	// ***** Skybox Stuff *****
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	
	renderer::bind(sky_eff);
	
	auto M = skybox.get_transform().get_transform_matrix();
	MVP = PV * M;
	
	glUniformMatrix4fv(sky_eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
	auto MVS = V * M;
	glUniformMatrix4fv(sky_eff.get_uniform_location("MV"), 1, GL_FALSE, value_ptr(MVS));
	glUniformMatrix4fv(sky_eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
	glUniformMatrix3fv(sky_eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(skybox.get_transform().get_normal_matrix()));
	glUniform3fv(sky_eff.get_uniform_location("eye_pos"), 1, value_ptr(cameras[cameraType]->get_position()));
	glUniform4fv(sky_eff.get_uniform_location("fog_colour"), 1, value_ptr(vec4(0.5f, 0.5f, 0.5f, 1.0f)));
	glUniform1f(sky_eff.get_uniform_location("fog_start"), 5.0f);
	glUniform1f(sky_eff.get_uniform_location("fog_end"), 5.0f);
	glUniform1f(sky_eff.get_uniform_location("fog_density"), 0.04f);
	glUniform1i(sky_eff.get_uniform_location("fog_type"), fog);
	
	glUniform1i(sky_eff.get_uniform_location("cubemap"), 0);

	renderer::bind(directLight, "light");

	renderer::render(skybox);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
	/***************************************************/

	/***** Terrain Stuff *****/
	renderer::bind(terrain_eff);
	M = terr.get_transform().get_transform_matrix();
	MVP = PV * M;
	glUniformMatrix4fv(terrain_eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
	auto MVT = V * M;
	glUniformMatrix4fv(terrain_eff.get_uniform_location("MV"), 1, GL_FALSE, value_ptr(MVT));
	glUniformMatrix4fv(terrain_eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
	glUniformMatrix3fv(terrain_eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(terr.get_transform().get_normal_matrix()));
	glUniform3fv(terrain_eff.get_uniform_location("eye_pos"), 1, value_ptr(cameras[cameraType]->get_position()));
	glUniform4fv(terrain_eff.get_uniform_location("fog_colour"), 1, value_ptr(vec4(0.5f, 0.5f, 0.5f, 1.0f)));
	glUniform1f(terrain_eff.get_uniform_location("fog_start"), 10.0f);
	glUniform1f(terrain_eff.get_uniform_location("fog_end"), 500.0f);
	glUniform1f(terrain_eff.get_uniform_location("fog_density"), 0.04f);
	glUniform1i(terrain_eff.get_uniform_location("fog_type"), fog);
	renderer::bind(terr.get_material(), "mat");

	renderer::bind(terrainTexs[0], 0);
	glUniform1i(terrain_eff.get_uniform_location("tex[0]"), 0);
	
	renderer::bind(terrainTexs[1], 1);
	glUniform1i(terrain_eff.get_uniform_location("tex[1]"), 1);
	
	renderer::bind(terrainTexs[2], 2);
	glUniform1i(terrain_eff.get_uniform_location("tex[2]"), 2);
	
	renderer::bind(terrainTexs[3], 3);
	glUniform1i(terrain_eff.get_uniform_location("tex[3]"), 3);

	renderer::bind(directLight, "light");

	renderer::render(terr);
	/***************************************************/


	for (auto &e : meshes) {
		auto m = e.second;
		// Bind effect
		renderer::bind(eff);
		// Create MVP matrix
		mat4 M;

		if (e.first == "TorusE" || e.first == "TorusF" || e.first == "TorusG" || e.first == "TorusH") {
			mat4 hierarchy_M = (*transformed_hierarchy[&e.second]).get_transform().get_transform_matrix();
			M = hierarchy_M * m.get_transform().get_transform_matrix();
		}
		else {
			M = m.get_transform().get_transform_matrix();
		}
		MVP = PV * M;
		// Set MVP matrix uniform
		glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
		auto MV = V * M;
		glUniformMatrix4fv(eff.get_uniform_location("MV"), 1, GL_FALSE, value_ptr(MV));
		glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
		glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(m.get_transform().get_normal_matrix()));
		renderer::bind(m.get_material(), "mat");
		renderer::bind(spotLight, "spot");
		renderer::bind(pointLights, "points");
		renderer::bind(directLight, "direct");

		renderer::bind(blankNormal, 1);
		if (e.first == "Roof") {
			renderer::bind(normalMap, 1);
		}

		renderer::bind(BindingHelper(e.first), 0);
		
		glUniform1i(eff.get_uniform_location("tex"), 0);
		glUniform1i(eff.get_uniform_location("normal_map"), 1);
		glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(cameras[cameraType]->get_position()));

		glUniform4fv(eff.get_uniform_location("fog_colour"), 1, value_ptr(vec4(0.5f, 0.5f, 0.5f, 1.0f)));
		glUniform1f(eff.get_uniform_location("fog_start"), 10.0f);
		glUniform1f(eff.get_uniform_location("fog_end"), 500.0f);
		glUniform1f(eff.get_uniform_location("fog_density"), 0.04f);
		glUniform1i(eff.get_uniform_location("fog_type"), fog);

		// Render geometry
		renderer::render(m);
	}

	if (glfwGetKey(renderer::get_window(), GLFW_KEY_LEFT_ALT)) {
		renderer::set_render_target(frames[current_frame]);
		renderer::clear();
		renderer::bind(motion_blur);
		MVP = mat4(1.0f);
		glUniformMatrix4fv(motion_blur.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
		renderer::bind(temp_frame.get_frame(), 0);
		renderer::bind(frames[(current_frame + 1) % 2].get_frame(), 1);
		glUniform1i(motion_blur.get_uniform_location("tex"), 0);
		glUniform1i(motion_blur.get_uniform_location("previous_frame"), 1);
		glUniform1f(motion_blur.get_uniform_location("blend_factor"), 0.9f);

		renderer::render(screen_quad);

		renderer::set_render_target();
		renderer::bind(frames[current_frame].get_frame(), 3);
		renderer::render(screen_quad);
	}

	if (glfwGetKey(renderer::get_window(), GLFW_KEY_RIGHT_CONTROL)) {
		renderer::set_render_target();
		renderer::bind(greyscale_eff);
		MVP = mat4(1.0f);
		glUniformMatrix4fv(greyscale_eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
		renderer::bind(greyscale_frame.get_frame(), 1);
		glUniform1i(greyscale_eff.get_uniform_location("tex"), 1);

		renderer::render(screen_quad);
	}

	if (glfwGetKey(renderer::get_window(), GLFW_KEY_0)) {
		renderer::set_render_target();
		renderer::bind(blur_eff);
		MVP = mat4(1.0f);
		glUniformMatrix4fv(blur_eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
		renderer::bind(greyscale_frame.get_frame(), 1);
		glUniform1i(blur_eff.get_uniform_location("tex"), 1);
		glUniform1f(blur_eff.get_uniform_location("inverse_width"), 5.0f / renderer::get_screen_width());
		glUniform1f(blur_eff.get_uniform_location("inverse_height"), 5.0f / renderer::get_screen_height());

		renderer::render(screen_quad);
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

  delete cameras[0];
  delete cameras[1];
}