//
// Created by skalimat on 17.10.19.
//

#ifndef AVT7_LIGHT_H
#define AVT7_LIGHT_H

#include "../GameObject.h"

class Light: public GameObject {
public:

    // 0 for directional
    // 1 for point
    float lightType = 0;

    explicit Light(Vector3 pos, float type): GameObject(pos) {
        this->lightType = type;
    }

    void init() override {
        // Do nothing
    }

    void render() override {
        float res[4];
        float lightPos[4] = {position.x, position.y, position.z, lightType};
        multMatrixPoint(VIEW, lightPos, res);
        glUniform4fv(lPos_uniformId, 1, res);
    }
};

#endif //AVT7_LIGHT_H
