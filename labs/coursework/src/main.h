#pragma once

#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

extern map<string, mesh> meshes;
extern map<mesh*, mesh*> transformed_hierarchy;
extern mesh skybox, terr;
extern geometry geom;
extern effect eff, sky_eff, terrain_eff, fog_eff;
extern map<string, texture> texs;
extern texture terrainTexs[4];
extern std::array<camera*, 2> cameras;
extern uint cameraType;
extern uint targetCam;
extern texture normalMap, blankNormal, modelTex;
extern material mat;
extern cubemap cube_map;
extern spot_light spotLight;
extern vector<point_light> pointLights;
extern directional_light directLight;
extern double cursor_x;
extern double cursor_y;
extern float temp;
extern int fog;