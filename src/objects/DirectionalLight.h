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

        setLightEnabled(light_enabled);
    }
};

#endif //AVT7_DIRECTIONALLIGHT_H
