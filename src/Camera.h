//
// Created by skalimat on 16.10.19.
//

#ifndef AVT7_CAMERA_H
#define AVT7_CAMERA_H

#include "Vector3.h"
#include "libs/AVTmathLib.h"

class Camera {
public:
    virtual void view() = 0;
    virtual void project(int w, int h) = 0;
};

#endif //AVT7_CAMERA_H
