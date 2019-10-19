//
// Created by skalimat on 19.10.19.
//

#include "AMaterial.h"

#ifndef AVT7_MATERIALS_H
#define AVT7_MATERIALS_H

float amb[4] = {0.1f, 0.1f, 0.1f, 1.0f};

float spec_shiny[4] = {0.8f, 0.8f, 0.8f, 1.0f};
float spec_matte[4] = {0.0f, 0.0f, 0.0f, 1.0f};

float red[4] = {1.0f, 0.0f, 0.0f, 1.0f};
float green[4] = {0.0f, 1.0f, 0.0f, 1.0f};
float blue[4] = {0.0f, 0.0f, 1.0f, 1.0f};
float yellow[4] = {1.0f, 1.0f, 0.0f, 1.0f};
float white[4] = {1.0f, 1.0f, 1.0f, 1.0f};
float black[4] = {0.0f, 0.0f, 0.0f, 1.0f};

float froggy_green[4] = {0.0f, 1.0f, 0.5f, 1.0f};

AMaterial mat_player    = AMaterial(amb, froggy_green, spec_matte, black, 10, 0);
AMaterial mat_bus       = AMaterial(amb, red, spec_matte, black, 10, 0);
AMaterial mat_ground    = AMaterial(amb, green, spec_matte, black, 10, 0);
AMaterial mat_river     = AMaterial(amb, blue, spec_matte, black, 10, 0);
AMaterial mat_road      = AMaterial(amb, black, spec_matte, black, 10, 0);
AMaterial mat_coord_x   = AMaterial(blue, blue, spec_matte, black, 10, 0);
AMaterial mat_coord_y   = AMaterial(yellow, yellow, spec_matte, black, 10, 0);
AMaterial mat_coord_z   = AMaterial(green, green, spec_matte, black, 10, 0);

#endif //AVT7_MATERIALS_H
