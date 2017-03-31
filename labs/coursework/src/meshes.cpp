#include <glm\glm.hpp>
#include <graphics_framework.h>
#include "meshes.h"
#include "main.h"

using namespace std;
using namespace graphics_framework;
using namespace glm;

void Meshes() {
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
	meshes["PillarFront"] = mesh(geometry_builder::create_cylinder(40, 40, vec3(5.0f, 55.0f, 5.0f)));
	meshes["PillarFrontB"] = mesh(geometry_builder::create_cylinder(40, 40, vec3(5.0f, 55.0f, 5.0f)));
	// ***** Move and Scale *****
	meshes["PillarFront"].get_transform().scale = vec3(5.0f);
	meshes["PillarFront"].get_transform().position = vec3(137.5f, 75.0f, 190.0f);
	meshes["PillarFrontB"].get_transform().scale = vec3(5.0f);
	meshes["PillarFrontB"].get_transform().position = vec3(-137.5f, 75.0f, 190.0f);
	//**********************************************************************************//

	// ***** Create Back Pillars *****
	meshes["PillarBack"] = mesh(geometry_builder::create_cylinder(40, 40, vec3(5.0f, 55.0f, 5.0f)));
	meshes["PillarBackB"] = mesh(geometry_builder::create_cylinder(40, 40, vec3(5.0f, 55.0f, 5.0f)));
	// ***** Move and Scale *****
	meshes["PillarBack"].get_transform().scale = vec3(5.0f);
	meshes["PillarBack"].get_transform().position = vec3(137.5f, 75.0f, -190.0f);
	meshes["PillarBackB"].get_transform().scale = vec3(5.0f);
	meshes["PillarBackB"].get_transform().position = vec3(-137.5f, 75.0f, -190.0f);
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

	// ***** Create Cog *****
	meshes["Cog"] = mesh(geometry("models/cog.obj"));
	meshes["Cog"].get_transform().scale = vec3(2000.0f);
	meshes["Cog"].get_transform().position = vec3(0.0f, 400.0f, 0.0f);
	//************************************************************************//

	/***** Create Box Underneath Building *****/
	meshes["WallFoundation"] = mesh(geometry_builder::create_box(vec3(250.0f, 61.0f, 400.0f)));
	meshes["WallFoundation"].get_transform().position = vec3(0.0f, -30.6f, 0.0f);
	//************************************************************************//
}