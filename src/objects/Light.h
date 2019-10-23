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
    int light_id;
    bool light_enabled;

protected:
    explicit Light(Vector3 pos, int light_id, bool light_active = true): GameObject(pos) {
        this->light_id = light_id;
        this->light_enabled = light_active;
    }
};

#endif //AVT7_LIGHT_H
