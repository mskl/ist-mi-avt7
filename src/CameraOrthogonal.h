//
// Created by skalimat on 17.10.19.
//

#ifndef AVT7_CAMERAORTHOGONAL_H
#define AVT7_CAMERAORTHOGONAL_H

#include "Camera.h"

class CameraOrthogonal: public Camera {
public:
    float left, right, top, bottom;

    CameraOrthogonal(float left, float right, float bottom, float top)
    : Camera(Vector3()) {
        this->left = left;
        this->right = right;
        this->top = top;
        this->bottom = bottom;
    }

    void view() final {
        lookAt(0, 10, 0, 0,0,0, 0,0,-1);
    }

    void project (int w, int h) final {
        ortho(left, right, bottom, top, -30, 30);
    }
};

#endif //AVT7_CAMERAORTHOGONAL_H
