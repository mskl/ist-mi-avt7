//
// Created by mykha on 17/10/19.
//

#ifndef AVT7_BOUNDINGBOX_H
#define AVT7_BOUNDINGBOX_H

#include "Vector3.h"

class BoundingBox {
protected:
    Vector3 vecMin;
    Vector3 vecMax;
public:
    BoundingBox(Vector3 min, Vector3 max) {
        this->vecMin = min;
        this->vecMax = max;
    }

    void setMin(Vector3 pos) {
        vecMin = pos;
    }
    void setMax(Vector3 pos) {
        vecMax = pos;
    }

    bool isColliding(const BoundingBox b, Vector3 deltaA = Vector3(), Vector3 deltaB = Vector3()) const {
        Vector3 aVecMax = vecMax + deltaA;
        Vector3 aVecMin = vecMin + deltaA;

        Vector3 bVecMax = b.vecMax + deltaB;
        Vector3 bVecMin = b.vecMin + deltaB;

        bool is_colliding = aVecMax.x > bVecMin.x &&
                            aVecMin.x < bVecMax.x &&
                            aVecMax.y > bVecMin.y &&
                            aVecMin.y < bVecMax.y &&
                            aVecMax.z > bVecMin.z &&
                            aVecMin.z < bVecMax.z;
        bool is_inside = isInside(b, deltaA, deltaB);
        return is_inside || is_colliding;
    }

    bool isInside(const BoundingBox b, Vector3 deltaA = Vector3(), Vector3 deltaB = Vector3()) const {
        Vector3 aVecMax = vecMax + deltaA;
        Vector3 aVecMin = vecMin + deltaA;

        Vector3 bVecMax = b.vecMax + deltaB;
        Vector3 bVecMin = b.vecMin + deltaB;

        return(aVecMax.x < bVecMax.x &&
               aVecMax.y < bVecMax.z &&
               aVecMax.z < bVecMax.y &&
               aVecMin.x > bVecMin.x &&
               aVecMin.y > bVecMin.z &&
               aVecMin.z > bVecMin.y);
    }
};
#endif //AVT7_BOUNDINGBOX_H