//
// Created by skalimat on 16.10.19.
//

#ifndef AVT7_CAMERAPERSPECTIVE_H
#define AVT7_CAMERAPERSPECTIVE_H

#include "Camera.h"

class CameraPerspective: public Camera {
public:
    float fov = 53.13f;

    float r = 20.0f;
    float alpha = 0.0f;
    float beta = 40.0f;

    Vector3 localPos = Vector3();

    CameraPerspective(float r, float alpha, float beta, Vector3 pos)
    : Camera(pos) {
        this->r = r;
        this->alpha = alpha;
        this->beta = beta;
        mouseUpdate(this->r, this->alpha, this->beta);
    }

    void mouseUpdate(float r, float alpha, float beta) {
        localPos.x = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
        localPos.z = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
        localPos.y = r * sin(beta  * 3.14f / 180.0f);
    }

    void view() override {
        lookAt(localPos.x + pos.x, localPos.y + pos.y, localPos.z + pos.z,
                pos.x, pos.y, pos.z,
                0, 1, 0);
    }

    void project(int w, int h) override {
        perspective(fov, (1.0f * w) / h, nearp, farp);
    }

    void printCoordinates() {
        printf("Camera Spherical Coordinates (%f, %f, %f)\n", alpha, beta, r);
    }

};

#endif //AVT7_CAMERAPERSPECTIVE_H
