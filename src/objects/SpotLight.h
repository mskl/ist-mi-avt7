//
// Created by skalimat on 23.10.19.
//

#ifndef AVT7_SPOTLIGHT_H
#define AVT7_SPOTLIGHT_H

#include "Light.h"
extern GLint l_spot_dir_id;

class SpotLight: public Light {
public:
    Vector3 light_dir;
    SpotLight (Vector3 pos, Vector3 dir, int light_id, bool light_active = true): Light(pos, light_id, light_active) {
        light_dir = dir;
    }

    void render() final {
        float res[4];
        float lightPosSpot[4] = {position.x, position.y, position.z, 1.0f};
        multMatrixPoint(VIEW, lightPosSpot, res);
        glUniform4fv(l_pos_id[light_id], 1, res);

        float lightDirSpot[4] = {light_dir.x, light_dir.y, light_dir.z, 0.0f};
        multMatrixPoint(VIEW, lightDirSpot, res);
        glUniform4fv(l_spot_dir_id, 1, res);

        // Set the light on or off and send the whole matrix to the GLSL
        l_enabled[light_id] = light_enabled;
        glUniform1iv(l_enabled_id, 8, l_enabled);
    }


};

#endif //AVT7_SPOTLIGHT_H
