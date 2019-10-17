//
// Created by mykha on 17/10/19.
//

#include "BoundingBox.h"

BoundingBox::BoundingBox(){

}

BoundingBox::~BoundingBox(){
}

bool BoundingBox::isColliding(BoundingBox b){
    bool is_inside = isInside(b);
    bool is_colliding = vecMax.x > b.vecMin.x &&
                        vecMin.x < b.vecMax.x &&
                        vecMax.y > b.vecMin.y &&
                        vecMin.y < b.vecMax.y &&
                        vecMax.z > b.vecMin.z &&
                        vecMin.z < b.vecMax.z;
    return is_inside || is_colliding;
}

bool BoundingBox::isInside(BoundingBox b){
    return(vecMax.x < b.vecMax.x &&
            vecMax.y < b.vecMax.z &&
            vecMax.z < b.vecMax.y &&
            vecMin.x > b.vecMin.x &&
            vecMin.y > b.vecMin.z &&
            vecMin.z > b.vecMin.y);

}


void BoundingBox::setMax(Vector3 tempPos){
    vecMax = tempPos;
}

void BoundingBox::setMin(Vector3 tempPos){
    vecMin = tempPos;
}

