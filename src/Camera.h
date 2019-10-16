//
// Created by skalimat on 16.10.19.
//

#ifndef AVT7_CAMERA_H
#define AVT7_CAMERA_H

#include "Vector3.h"
#include "libs/AVTmathLib.h"

class Camera {
public:
    Vector3 pos;  // float camX, camY, camZ;

    Camera(float r, float alpha, float beta) {
        // set the camera position based on its spherical coordinates
        this->pos = Vector3();
        mouseUpdate(r, alpha, beta);
    }

    void renderCam() {
        lookAt(pos.getX(), pos.getY(), pos.getZ(), 0, 0, 0, 0, 1, 0);
    }

    void mouseUpdate(float r, float alpha, float beta) {
        pos.x = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
        pos.z = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
        pos.y = r * sin(beta * 3.14f / 180.0f);
    }
};

#endif //AVT7_CAMERA_H
