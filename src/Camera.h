//
// Created by skalimat on 16.10.19.
//

#ifndef AVT7_CAMERA_H
#define AVT7_CAMERA_H

#include "Vector3.h"
#include "libs/AVTmathLib.h"

class Camera {
public:
    float nearp = 0.1f;
    float farp = 1000.0f;
    Vector3 pos;

    explicit Camera(Vector3 pos) {
        pos = pos;
    }

    virtual void view() = 0;
    virtual void project(int w, int h) = 0;
};

#endif //AVT7_CAMERA_H
