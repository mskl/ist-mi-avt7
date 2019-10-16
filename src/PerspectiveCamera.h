//
// Created by skalimat on 16.10.19.
//

#ifndef AVT7_PERSPECTIVECAMERA_H
#define AVT7_PERSPECTIVECAMERA_H

#include "Camera.h"

class PerspectiveCamera: Camera {
public:
    Vector3 pos;

    PerspectiveCamera(float r, float alpha, float beta) {
        this->pos = Vector3();
        mouseUpdate(r, alpha, beta);
    }

    void mouseUpdate(float r, float alpha, float beta) {
        pos.x = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
        pos.z = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
        pos.y = r * sin(beta * 3.14f / 180.0f);
    }

    void view()  {
        lookAt(pos.x, pos.y, pos.z, 0, 0, 0, 0, 1, 0);
    }

    void project(int w, int h) {
        perspective(53.13f, (1.0f * w) / h, 0.1f, 1000.0f);
    }

};

#endif //AVT7_PERSPECTIVECAMERA_H
