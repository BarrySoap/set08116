#pragma once

#include <glm\glm.hpp>
#include <graphics_framework.h>

using namespace std;
using namespace graphics_framework;
using namespace glm;

void generate_terrain(geometry &geom, const texture &height_map, unsigned int width, unsigned int depth, float height_scale);