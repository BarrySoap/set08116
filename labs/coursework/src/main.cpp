#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

map<string, mesh> meshes;
map<mesh*, mesh*> transformed_hierarchy;
mesh skybox;
geometry geom;
effect eff;
effect sky_eff;
map<string, texture> texs;
array<camera*, 2> cameras;
uint cameraType = 1;
uint targetCam = 1;
texture normalMap;
texture blankNormal;
material mat;
cubemap cube_map;
spot_light spotLight;
vector<point_light> pointLights(6);
directional_light directLight;
double cursor_x = 0.0;
double cursor_y = 0.0;

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
	// ***** Create Plane/Flooring *****
	meshes["Floor"] = mesh(geometry_builder::create_plane());
	meshes["FloorB"] = mesh(geometry_builder::create_box(vec3(49.0f, 2.0f, 10.0f)));
	meshes["Carpet"] = mesh(geometry_builder::create_box(vec3(10.0f, 1.0f, 72.0f)));
	// ***** Move and Scale *****
	meshes["Floor"].get_transform().scale = vec3(2.5f, 1.0f, 4.0f);
	meshes["FloorB"].get_transform().scale = vec3(5.0f);
	meshes["FloorB"].get_transform().position = vec3(0.0f, 5.0f, -170.0f);
	meshes["Carpet"].get_transform().scale = vec3(5.0f, 1.0f, 5.0f);
	meshes["Carpet"].get_transform().position = vec3(0.0f, 0.0f, 10.0f);
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
	meshes["Stand"] = mesh(geometry_builder::create_cylinder(40, 40, vec3(2.0f, 20.0f, 2.0f)));
	meshes["StandB"] = mesh(geometry_builder::create_cylinder(40, 40, vec3(2.0f, 20.0f, 2.0f)));
	meshes["StandC"] = mesh(geometry_builder::create_cylinder(40, 40, vec3(2.0f, 20.0f, 2.0f)));
	meshes["StandD"] = mesh(geometry_builder::create_cylinder(40, 40, vec3(2.0f, 20.0f, 2.0f)));
	// ***** Move and Scale *****
	meshes["Stand"].get_transform().scale = vec3(5.0f);
	meshes["Stand"].get_transform().position = vec3(70.0f, 50.0f, 80.0f);
	meshes["StandB"].get_transform().scale = vec3(5.0f);
	meshes["StandB"].get_transform().position = vec3(-70.0f, 50.0f, 80.0f);
	meshes["StandC"].get_transform().scale = vec3(5.0f);
	meshes["StandC"].get_transform().position = vec3(70.0f, 50.0f, -80.0f);
	meshes["StandD"].get_transform().scale = vec3(5.0f);
	meshes["StandD"].get_transform().position = vec3(-70.0f, 50.0f, -80.0f);
	//*************************************************************************//

	// ***** Create Torus' *****
	meshes["Torus"] = mesh(geometry_builder::create_torus(20, 20, 1.0f, 3.0f));
	meshes["TorusB"] = mesh(geometry_builder::create_torus(20, 20, 1.0f, 3.0f));
	meshes["TorusC"] = mesh(geometry_builder::create_torus(20, 20, 1.0f, 3.0f));
	meshes["TorusD"] = mesh(geometry_builder::create_torus(20, 20, 1.0f, 3.0f));
	meshes["TorusE"] = mesh(geometry_builder::create_torus(20, 20, 1.0f, 3.0f));
	meshes["TorusF"] = mesh(geometry_builder::create_torus(20, 20, 1.0f, 3.0f));
	meshes["TorusG"] = mesh(geometry_builder::create_torus(20, 20, 1.0f, 3.0f));
	meshes["TorusH"] = mesh(geometry_builder::create_torus(20, 20, 1.0f, 3.0f));
	// ***** Move and Scale *****
	meshes["Torus"].get_transform().scale = vec3(5.0f);
	meshes["Torus"].get_transform().position = vec3(70.0f, 5.0f, 80.0f);
	meshes["TorusB"].get_transform().scale = vec3(5.0f);
	meshes["TorusB"].get_transform().position = vec3(-70.0f, 5.0f, 80.0f);
	meshes["TorusC"].get_transform().scale = vec3(5.0f);
	meshes["TorusC"].get_transform().position = vec3(70.0f, 5.0f, -80.0f);
	meshes["TorusD"].get_transform().scale = vec3(5.0f);
	meshes["TorusD"].get_transform().position = vec3(-70.0f, 5.0f, -80.0f);
	//************************************************************************//

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

	// ***** Set Material Attributes *****
	mat.set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	mat.set_specular(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	mat.set_shininess(25.0f);
	mat.set_diffuse(vec4(0.5f, 0.5f, 0.5f, 1.0f));

	meshes["Floor"].set_material(mat);
	meshes["FloorB"].set_material(mat);
	meshes["WallFrontA"].set_material(mat);
	meshes["WallFrontB"].set_material(mat);
	meshes["WallFrontAB"].set_material(mat);
	meshes["WallRight"].set_material(mat);
	meshes["WallLeft"].set_material(mat);
	meshes["WallRight"].set_material(mat);
	meshes["WallTop"].set_material(mat);
	meshes["WallBack"].set_material(mat);
	meshes["Roof"].set_material(mat);

	mat.set_specular(vec4(0.5f, 0.5f, 0.5f, 1.0f));
	mat.set_diffuse(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["Carpet"].set_material(mat);

	mat.set_specular(vec4(0.8f, 0.8f, 0.8f, 1.0f));
	mat.set_diffuse(vec4(0.5f, 0.5f, 0.5f, 1.0f));
	meshes["PillarFront"].set_material(mat);
	meshes["PillarFrontB"].set_material(mat);
	meshes["PillarBack"].set_material(mat);
	meshes["PillarBackB"].set_material(mat);
	meshes["Stand"].set_material(mat);
	meshes["StandB"].set_material(mat);
	meshes["StandC"].set_material(mat);
	meshes["StandD"].set_material(mat);

	mat.set_specular(vec4(0.3f, 0.3f, 0.3f, 1.0f));
	mat.set_diffuse(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	meshes["Torus"].set_material(mat);
	meshes["TorusB"].set_material(mat);
	meshes["TorusC"].set_material(mat);
	meshes["TorusD"].set_material(mat);
	meshes["TorusE"].set_material(mat);
	meshes["TorusF"].set_material(mat);
	meshes["TorusG"].set_material(mat);
	meshes["TorusH"].set_material(mat);
	/*********************************************/

	// ***** Initialise Textures *****
	texs["Wall"]  = texture("textures/InsideWall.jpg", true, true);
	texs["Floor"] = texture("textures/Floor.jpg", true, true);
	texs["Roof"] = texture("textures/Roof.jpg", true, true);
	texs["Pillar"] = texture("textures/Pillar.jpg", true, true);
	texs["Stand"] = texture("textures/Pillar.jpg", true, true);
	texs["Torus"] = texture("textures/Torus.jpg", true, true);
	texs["Carpet"] = texture("textures/Carpet.jpg", true, true);

	normalMap = texture("textures/RoofNormalMap.jpg", true, true);
	blankNormal = texture("textures/BlankNormal.jpg", true, true);
	array<string, 6> filenames = { "textures/sahara_ft.jpg", "textures/sahara_bk.jpg", "textures/sahara_up.jpg",
		"textures/sahara_dn.jpg", "textures/sahara_rt.jpg", "textures/sahara_lf.jpg" };
	/*************************************************************/

	// ***** Set Light Attributes *****
	spotLight.set_position(vec3(0.0f, 10.0f, 150.0f));
	spotLight.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	spotLight.set_direction(normalize(vec3(0.0f, 0.0f, -1.0f)));
	spotLight.set_range(20.0f);
	spotLight.set_power(1.0f);

	for (int i = 0; i < 6; ++i) {
		pointLights[i].set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		pointLights[i].set_range(20.0f);
	}

	pointLights[0].set_position(vec3(0.0f, 270.0f, 0));
	pointLights[1].set_position(vec3(137.5f, 270.0f, 190.0f));
	pointLights[2].set_position(vec3(-137.5f, 270.0f, 190.0f));
	pointLights[3].set_position(vec3(137.5f, 270.0f, -190.0f));
	pointLights[4].set_position(vec3(-137.5f, 270.0f, -190.0f));
	pointLights[5].set_position(vec3(0.0f, 20.0f, 250.0f));

	directLight.set_ambient_intensity(vec4(0.1f, 0.1f, 0.1f, 1.0f));
	directLight.set_direction(normalize(vec3(0.0f, 1.0f, 0.0f)));
	directLight.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	/******************************************************/

	// ***** Create cube map *****
	cube_map = cubemap(filenames);
	/****************************/

  // ***** Load In Shaders *****
  eff.add_shader("shaders/main.vert", GL_VERTEX_SHADER);
  vector<string> frag_shaders{ "shaders/main.frag", "shaders/part_point.frag", "shaders/part_spot.frag", "shaders/part_normal.frag"};
  eff.add_shader(frag_shaders, GL_FRAGMENT_SHADER);

  sky_eff.add_shader("shaders/skybox.vert", GL_VERTEX_SHADER);
  sky_eff.add_shader("shaders/skybox.frag", GL_FRAGMENT_SHADER);
  sky_eff.build();
  /****************************************************************/

  // Build Effect
  eff.build();

  // ***** Set Free Camera (Default) Properties *****
  cameras[1]->set_position(vec3(0.0f, 10.0f, 400.0f));
  cameras[1]->set_target(vec3(0.0f, 0.0f, 0.0f));
  cameras[1]->set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
  auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
  /*****************************************************************************************************************/
  return true;
}


bool update(float delta_time) {
	skybox.get_transform().position = cameras[1]->get_position();
	cout << 1.0f / delta_time << endl;
	static float range = 70.0f;

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

	return true;
}

texture BindingHelper(string name) {
	// If the texture name contains the string 'Pillar'
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
	else {
		return texs[name];
	}
}

bool render() {
	// ***** Skybox Stuff *****
	glDisable(GL_DEPTH_TEST);
	glDepthMask(GL_FALSE);
	glDisable(GL_CULL_FACE);
	
	renderer::bind(sky_eff);
	
	auto M = skybox.get_transform().get_transform_matrix();
	auto V = cameras[cameraType]->get_view();
	auto P = cameras[cameraType]->get_projection();
	auto MVP = P * V * M;
	
	glUniformMatrix4fv(sky_eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
	
	glUniform1i(sky_eff.get_uniform_location("cubemap"), 0);

	renderer::render(skybox);

	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glEnable(GL_CULL_FACE);
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
		auto V = cameras[cameraType]->get_view();
		auto P = cameras[cameraType]->get_projection();
		auto MVP = P * V * M;
		// Set MVP matrix uniform
		glUniformMatrix4fv(eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVP));
		glUniformMatrix4fv(eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(M));
		glUniformMatrix3fv(eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(m.get_transform().get_normal_matrix()));
		renderer::bind(m.get_material(), "mat");
		renderer::bind(spotLight, "spot");
		renderer::bind(pointLights, "points");
		renderer::bind(directLight, "direct");

		glUniform1i(eff.get_uniform_location("blankNormal"), 1);
		if (e.first == "Roof") {
			renderer::bind(normalMap, 1);
		}

		renderer::bind(BindingHelper(e.first), 0);
		renderer::bind(normalMap, 1);
		
		glUniform1i(eff.get_uniform_location("tex"), 0);
		glUniform1i(eff.get_uniform_location("normalMap"), 2);
		glUniform3fv(eff.get_uniform_location("eye_pos"), 1, value_ptr(cameras[cameraType]->get_position()));

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

  delete cameras[0];
  delete cameras[1];
}