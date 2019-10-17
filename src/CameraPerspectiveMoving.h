//
// Created by skalimat on 17.10.19.
//

#ifndef AVT7_CAMERAPERSPECTIVEMOVING_H
#define AVT7_CAMERAPERSPECTIVEMOVING_H

#include "CameraPerspective.h"

class CameraPerspectiveMoving: public CameraPerspective {
public:
    int startX = 0, startY = 0, tracking = 0;

    CameraPerspectiveMoving(float r, float alpha, float beta): CameraPerspective(r, alpha, beta) {

    }

    void processMouseButtons(int button, int state, int xx, int yy)
    {
        // start tracking the mouse
        if (state == GLUT_DOWN)  {
            startX = xx;
            startY = yy;
            if (button == GLUT_LEFT_BUTTON)
                tracking = 1;
            else if (button == GLUT_RIGHT_BUTTON)
                tracking = 2;
        }

        // stop tracking the mouse
        else if (state == GLUT_UP) {
            if (tracking == 1) {
                alpha -= xx - startX;
                beta += yy - startY;
            }
            else if (tracking == 2) {
                r += (yy - startY) * 0.01f;
                if (r < 0.1f)
                    r = 0.1f;
            }
            tracking = 0;
        }
    }

    void processMouseMotion(int xx, int yy)
    {
        int deltaX = 0, deltaY = 0;
        float alphaAux = 0, betaAux = 0, rAux = 0;

        deltaX = -xx + startX;
        deltaY =  yy - startY;

        // left mouse button: move cam
        if (tracking == 1) {
            alphaAux = alpha + deltaX;
            betaAux = beta + deltaY;

            if (betaAux > 85.0f) betaAux = 85.0f;
            else if (betaAux < -85.0f) betaAux = -85.0f;
            rAux = r;
        }
        // right mouse button: zoom
        else if (tracking == 2) {
            alphaAux = alpha;
            betaAux = beta;
            rAux = r + (deltaY * 0.01f);
            if (rAux < 0.1f) rAux = 0.1f;
        }

        mouseUpdate(rAux, alphaAux, betaAux);
    }

    void mouseWheel(int wheel, int direction, int x, int y)
    {
        r += direction * 0.1f;
        if (r < 0.1f)
            r = 0.1f;

        mouseUpdate(r, alpha, beta);
    }

};

#endif //AVT7_CAMERAPERSPECTIVEMOVING_H
