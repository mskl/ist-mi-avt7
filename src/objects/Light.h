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

    void init() override {
        // Upload the light types uniform matrix into the GLSL
        l_type[light_id] = lightType;
        glUniform1iv(l_type_id, 8, l_type);
    }

protected:
    explicit Light(Vector3 pos, LightType type, int light_id, bool light_active = true): GameObject(pos) {
        this->lightType = type;
        this->light_id = light_id;
        this->light_enabled = light_active;
    }
};

#endif //AVT7_LIGHT_H
