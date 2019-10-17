//
// Created by skalimat on 17.10.19.
//

#ifndef AVT7_CAMERAORTHOGONAL_H
#define AVT7_CAMERAORTHOGONAL_H

#include "Camera.h"

class CameraOrthogonal: public Camera {
public:
    float left, right, top, bottom;

    CameraOrthogonal(float left, float right, float bottom, float top, Vector3 pos)
    : Camera(pos) {
        this->left = left;
        this->right = right;
        this->top = top;
        this->bottom = bottom;
    }

    void view() override {
        // TODO: FIX POS NOT BEING TAKEN FROM CONSTRUCTOR
        lookAt(0, 20, 0, 0, 0, -0.000000000000000001, 0, 1, 0);
    }

    void project (int w, int h) override {
        ortho(left, right, bottom, top, nearp, farp);
    }

};

#endif //AVT7_CAMERAORTHOGONAL_H
