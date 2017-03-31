#include <glm\glm.hpp>
#include <graphics_framework.h>
#include "main.h"
#include "meshes.h"

using namespace std;
using namespace graphics_framework;
using namespace glm;

map<string, mesh> meshes;
map<mesh*, mesh*> transformed_hierarchy;
mesh skybox;
mesh terr;
geometry geom;
effect eff;
effect sky_eff;
effect terrain_eff;
map<string, texture> texs;
texture terrainTexs[4];
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

void generate_terrain(geometry &geom, const texture &height_map, unsigned int width, unsigned int depth,
	float height_scale) {
	// Contains our position data
	vector<vec3> positions;
	// Contains our normal data
	vector<vec3> normals;
	// Contains our texture coordinate data
	vector<vec2> tex_coords;
	// Contains our texture weights
	vector<vec4> tex_weights;
	// Contains our index data
	vector<unsigned int> indices;

	// Extract the texture data from the image
	glBindTexture(GL_TEXTURE_2D, height_map.get_id());
	auto data = new vec4[height_map.get_width() * height_map.get_height()];
	glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_FLOAT, (void *)data);

	// Determine ratio of height map to geometry
	float width_point = static_cast<float>(width) / static_cast<float>(height_map.get_width());
	float depth_point = static_cast<float>(depth) / static_cast<float>(height_map.get_height());

	// Point to work on
	vec3 point;

	// Part 1 - Iterate through each point, calculate vertex and add to vector
	for (int x = 0; x < height_map.get_width(); ++x) {
		// Calculate x position of point
		point.x = -(width / 2.0f) + (width_point * static_cast<float>(x));

		for (int z = 0; z < height_map.get_height(); ++z) {
			// *********************************
			// Calculate z position of point
			point.z = -(depth / 2.0f) + (depth_point * static_cast<float>(z));
			// *********************************
			// Y position based on red component of height map data
			point.y = data[(z * height_map.get_width()) + x].y * height_scale;
			// Add point to position data
			positions.push_back(point);
		}
	}

	// Part 1 - Add index data
	for (unsigned int x = 0; x < height_map.get_width() - 1; ++x) {
		for (unsigned int y = 0; y < height_map.get_height() - 1; ++y) {
			// Get four corners of patch
			unsigned int top_left = (y * height_map.get_width()) + x;
			unsigned int top_right = (y * height_map.get_width()) + x + 1;
			// *********************************
			unsigned int bottom_left = ((y + 1) * height_map.get_width()) + x;
			unsigned int bottom_right = ((y + 1) * height_map.get_height()) + (x + 1);
			// *********************************
			// Push back indices for triangle 1 (tl,br,bl)
			indices.push_back(top_left);
			indices.push_back(bottom_right);
			indices.push_back(bottom_left);
			// Push back indices for triangle 2 (tl,tr,br)
			// *********************************
			indices.push_back(top_left);
			indices.push_back(top_right);
			indices.push_back(bottom_right);
			// *********************************
		}
	}

	// Resize the normals buffer
	normals.resize(positions.size());

	// Part 2 - Calculate normals for the height map
	for (unsigned int i = 0; i < indices.size() / 3; ++i) {
		// Get indices for the triangle
		auto idx1 = indices[i * 3];
		auto idx2 = indices[i * 3 + 1];
		auto idx3 = indices[i * 3 + 2];

		// Calculate two sides of the triangle
		vec3 side1 = positions[idx1] - positions[idx3];
		vec3 side2 = positions[idx1] - positions[idx2];

		// Normal is normal(cross product) of these two sides
		// *********************************
		vec3 n = normalize(cross(side2, side1));
		// Add to normals in the normal buffer using the indices for the triangle
		normals[idx1] = normals[idx1] + n;
		normals[idx2] = normals[idx2] + n;
		normals[idx3] = normals[idx3] + n;
		// *********************************
	}

	// Normalize all the normals
	for (auto &n : normals) {
		// *********************************
		normalize(n);
		// *********************************
	}

	// Part 3 - Add texture coordinates for geometry
	for (unsigned int x = 0; x < height_map.get_width(); ++x) {
		for (unsigned int z = 0; z < height_map.get_height(); ++z) {
			tex_coords.push_back(vec2(width_point * x, depth_point * z));
		}
	}

	// Part 4 - Calculate texture weights for each vertex
	for (unsigned int x = 0; x < height_map.get_width(); ++x) {
		for (unsigned int z = 0; z < height_map.get_height(); ++z) {
			// Calculate tex weight
			vec4 tex_weight(clamp(1.0f - abs(data[(height_map.get_width() * z) + x].y - 0.0f) / 0.25f, 0.0f, 1.0f),
				clamp(1.0f - abs(data[(height_map.get_width() * z) + x].y - 0.15f) / 0.25f, 0.0f, 1.0f),
				clamp(1.0f - abs(data[(height_map.get_width() * z) + x].y - 0.5f) / 0.25f, 0.0f, 1.0f),
				clamp(1.0f - abs(data[(height_map.get_width() * z) + x].y - 0.9f) / 0.25f, 0.0f, 1.0f));

			// *********************************
			// Sum the components of the vector
			float texSum = tex_weight.x + tex_weight.y + tex_weight.z + tex_weight.w;
			// Divide weight by sum
			vec4 w = tex_weight / texSum;
			// Add tex weight to weights
			tex_weights.push_back(tex_weight);
			// *********************************
		}
	}

	// Add necessary buffers to the geometry
	geom.add_buffer(positions, BUFFER_INDEXES::POSITION_BUFFER);
	geom.add_buffer(normals, BUFFER_INDEXES::NORMAL_BUFFER);
	geom.add_buffer(tex_coords, BUFFER_INDEXES::TEXTURE_COORDS_0);
	geom.add_buffer(tex_weights, BUFFER_INDEXES::TEXTURE_COORDS_1);
	geom.add_index_buffer(indices);

	// Delete data
	delete[] data;
}

bool load_content() {

	Meshes();

	texture height_map("textures/heightmap.jpg");

	generate_terrain(geom, height_map, 3000, 3000, 500.0f);

	terr = mesh(geom);

	terr.get_transform().translate(vec3(0.0f, -300.0f, -600.0f));

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

	terr.get_material().set_diffuse(vec4(0.5f, 0.5f, 0.5f, 1.0f));
	terr.get_material().set_specular(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	terr.get_material().set_shininess(20.0f);
	terr.get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	/*********************************************/

	// ***** Initialise Textures *****
	texs["Wall"]  = texture("textures/InsideWall.jpg", true, true);
	texs["Floor"] = texture("textures/Floor.jpg", true, true);
	texs["Roof"] = texture("textures/Roof.jpg", true, true);
	texs["Pillar"] = texture("textures/Pillar.jpg", true, true);
	texs["Stand"] = texture("textures/Pillar.jpg", true, true);
	texs["Torus"] = texture("textures/Torus.jpg", true, true);
	texs["Carpet"] = texture("textures/Carpet.jpg", true, true);
	terrainTexs[0] = texture("textures/sand.jpg");
	terrainTexs[1] = texture("textures/grass.jpg");
	terrainTexs[2] = texture("textures/stone.jpg");
	terrainTexs[3] = texture("textures/snow.jpg");

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

  terrain_eff.add_shader("shaders/terrain.vert", GL_VERTEX_SHADER);
  terrain_eff.add_shader("shaders/terrain.frag", GL_FRAGMENT_SHADER);
  terrain_eff.add_shader("shaders/part_direction.frag", GL_FRAGMENT_SHADER);
  terrain_eff.add_shader("shaders/part_weighted_texture_4.frag", GL_FRAGMENT_SHADER);
  // Build effect
  terrain_eff.build();
  /****************************************************************/

  // Build Effect
  eff.build();

  // ***** Set Free Camera (Default) Properties *****
  cameras[1]->set_position(vec3(0.0f, 10.0f, 400.0f));
  cameras[1]->set_target(vec3(0.0f, 0.0f, 0.0f));
  cameras[1]->set_projection(quarter_pi<float>(), renderer::get_screen_aspect(), 0.1f, 1000.0f);
  auto aspect = static_cast<float>(renderer::get_screen_width()) / static_cast<float>(renderer::get_screen_height());
  /*****************************************************************************************************************/

  terr.get_transform().translate(vec3(0.0f, -40.0f, 0.0f));

  return true;
}


bool update(float delta_time) {
	
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
	skybox.get_transform().position = cameras[cameraType]->get_position();

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

	/***** Terrain Stuff *****/
	renderer::bind(terrain_eff);
	auto MT = terr.get_transform().get_transform_matrix();
	auto VT = cameras[cameraType]->get_view();
	auto PT = cameras[cameraType]->get_projection();
	auto MVPT = PT * VT * MT;
	glUniformMatrix4fv(terrain_eff.get_uniform_location("MVP"), 1, GL_FALSE, value_ptr(MVPT));
	glUniformMatrix4fv(terrain_eff.get_uniform_location("M"), 1, GL_FALSE, value_ptr(MT));
	glUniformMatrix3fv(terrain_eff.get_uniform_location("N"), 1, GL_FALSE, value_ptr(terr.get_transform().get_normal_matrix()));
	glUniform3fv(terrain_eff.get_uniform_location("eye_pos"), 1, value_ptr(cameras[cameraType]->get_position()));
	renderer::bind(terr.get_material(), "mat");

	renderer::bind(terrainTexs[0], 0);
	glUniform1i(eff.get_uniform_location("tex[0]"), 0);
	
	renderer::bind(terrainTexs[1], 1);
	glUniform1i(eff.get_uniform_location("tex[1]"), 1);
	
	renderer::bind(terrainTexs[2], 2);
	glUniform1i(eff.get_uniform_location("tex[2]"), 2);
	
	renderer::bind(terrainTexs[3], 3);
	glUniform1i(eff.get_uniform_location("tex[3]"), 3);

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