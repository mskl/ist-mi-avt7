//
// Created by skalimat on 23.10.19.
//

#ifndef AVT7_DIRECTIONALLIGHT_H
#define AVT7_DIRECTIONALLIGHT_H

#include "Light.h"
#include "../libs/AVTmathLib.h"

class DirectionalLight: public Light {
public:
    DirectionalLight(Vector3 pos, int light_id, bool light_active = true): Light(pos, light_id, light_active) {

    }

    void render() final {
        float res[4];
        float lightPos[4] = {position.x, position.y, position.z, 0.0f};
        multMatrixPoint(VIEW, lightPos, res);
        glUniform4fv(l_pos_id[light_id], 1, res);

        // Set the light on or off and send the whole matrix to the GLSL
        l_enabled[light_id] = light_enabled;
        glUniform1iv(l_enabled_id, 8, l_enabled);
    }
};

#endif //AVT7_DIRECTIONALLIGHT_H
