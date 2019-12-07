//
// Created by skalimat on 17.10.19.
//

#ifndef AVT7_LIGHT_H
#define AVT7_LIGHT_H

#include "../GameObject.h"

extern GLint l_enabled_id;
extern GLint l_enabled[8];

class Light: public GameObject {
public:
    void toggleLight() {
        light_enabled = !light_enabled;
    }
    bool isEnabled(){
        return light_enabled;
    }

protected:
    bool light_enabled;
    int light_id;

    explicit Light(Vector3 pos, int light_id, bool light_active = true): GameObject(pos) {
        this->light_id = light_id;
        this->light_enabled = light_active;
    }

    void setLightEnabled(bool newState) {
        light_enabled = newState;

        // Set the light on or off and send the whole matrix to the GLSL
        l_enabled[light_id] = light_enabled;
        glUniform1iv(l_enabled_id, 8, l_enabled);
    }

};

#endif //AVT7_LIGHT_H
