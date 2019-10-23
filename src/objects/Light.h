//
// Created by skalimat on 17.10.19.
//

#ifndef AVT7_LIGHT_H
#define AVT7_LIGHT_H

#include "../GameObject.h"

extern GLint l_enabled_id;
extern GLint l_enabled[8];

extern GLint l_type_id;
extern GLint l_type[8];

enum LightType {DIRECTIONAL = 0, POINT = 1, SPOT = 2};

class Light: public GameObject {
public:

    LightType lightType;
    int light_id;
    bool light_enabled;

    explicit Light(Vector3 pos, LightType type, int light_id, bool light_active = true): GameObject(pos) {
        this->lightType = type;
        this->light_id = light_id;
        this->light_enabled = light_active;
    }

    void init() override {
        // Upload the light types uniform matrix into the GLSL
        l_type[light_id] = lightType;
        glUniform1iv(l_type_id, 8, l_type);
    }

    void render() override {
        float res[4];
        float lightPos[4] = {position.x, position.y, position.z, 1.0f};
        if (lightType == POINT) {
            lightPos[3] = 1;
        } else if (lightType == DIRECTIONAL) {
            lightPos[3] = 0;
        }

        multMatrixPoint(VIEW, lightPos, res);
        glUniform4fv(l_pos_id[light_id], 1, res);

        // Set the light on or off and send the whole matrix to the GLSL
        l_enabled[light_id] = light_enabled;
        glUniform1iv(l_enabled_id, 8, l_enabled);
    }
};

#endif //AVT7_LIGHT_H
