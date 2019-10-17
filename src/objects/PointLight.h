//
// Created by skalimat on 17.10.19.
//

#ifndef AVT7_POINTLIGHT_H
#define AVT7_POINTLIGHT_H

#include "../GameObject.h"

class PointLight: public GameObject {
public:
    void init() {
        position = Vector3(4.0f, 6.0f, 2.0f);
    }

    void render() {
        float res[4];
        float lightPos[4] = {position.x, position.y, position.z, 1};
        multMatrixPoint(VIEW, lightPos, res);
        glUniform4fv(lPos_uniformId, 1, res);
    }
};

#endif //AVT7_POINTLIGHT_H
