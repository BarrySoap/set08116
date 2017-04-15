#include <glm\glm.hpp>
#include <graphics_framework.h>
#include "lightsAndMaterials.h"
#include "main.h"

using namespace std;
using namespace graphics_framework;
using namespace glm;

void SetMaterials() {
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

	terr.get_material().set_diffuse(vec4(0.1f, 0.1f, 0.1f, 1.0f));
	terr.get_material().set_specular(vec4(0.0f, 0.0f, 0.0f, 1.0f));
	terr.get_material().set_shininess(1.0f);
	terr.get_material().set_emissive(vec4(0.0f, 0.0f, 0.0f, 1.0f));
}

void SetLights() {
	spotLight.set_position(vec3(0.0f, 10.0f, 150.0f));
	spotLight.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
	spotLight.set_direction(normalize(vec3(0.0f, 0.0f, -1.0f)));
	spotLight.set_range(20.0f);
	spotLight.set_power(1.0f);

	for (int i = 0; i < 7; ++i) {
		pointLights[i].set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
		pointLights[i].set_range(20.0f);
	}

	pointLights[0].set_position(vec3(0.0f, 270.0f, 0));
	pointLights[1].set_position(vec3(137.5f, 270.0f, 190.0f));
	pointLights[2].set_position(vec3(-137.5f, 270.0f, 190.0f));
	pointLights[3].set_position(vec3(137.5f, 270.0f, -190.0f));
	pointLights[4].set_position(vec3(-137.5f, 270.0f, -190.0f));
	pointLights[5].set_position(vec3(0.0f, 20.0f, 250.0f));
	pointLights[6].set_position(vec3(1250.0f, 25.0f, 1250.0f));

	directLight.set_ambient_intensity(vec4(0.1f, 0.1f, 0.1f, 1.0f));
	directLight.set_direction(normalize(vec3(0.0f, 1.0f, 0.0f)));
	directLight.set_light_colour(vec4(1.0f, 1.0f, 1.0f, 1.0f));
}