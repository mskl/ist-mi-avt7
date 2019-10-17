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

    bool isColliding(BoundingBox b) {
        bool is_inside = isInside(b);
        bool is_colliding = vecMax.x > b.vecMin.x &&
                            vecMin.x < b.vecMax.x &&
                            vecMax.y > b.vecMin.y &&
                            vecMin.y < b.vecMax.y &&
                            vecMax.z > b.vecMin.z &&
                            vecMin.z < b.vecMax.z;
        return is_inside || is_colliding;
    }

    bool isInside(BoundingBox b) {
        return(vecMax.x < b.vecMax.x &&
               vecMax.y < b.vecMax.z &&
               vecMax.z < b.vecMax.y &&
               vecMin.x > b.vecMin.x &&
               vecMin.y > b.vecMin.z &&
               vecMin.z > b.vecMin.y);
    }
};
#endif //AVT7_BOUNDINGBOX_H