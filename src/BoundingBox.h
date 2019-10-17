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
    BoundingBox();
    ~BoundingBox();
    void setMin(Vector3 pos);
    void setMax(Vector3 pos);
    bool isColliding(BoundingBox b);
    bool isInside(BoundingBox b);
};
#endif //AVT7_BOUNDINGBOX_H