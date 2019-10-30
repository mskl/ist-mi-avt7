//
// Created by mykha on 22.10.19.
//

#ifndef AVT7_SIDECOLLIDER_H
#define AVT7_SIDECOLLIDER_H

#include "../DynamicGameObject.h"

enum SideColliderSide {LEFT, RIGHT};

class SideCollider: public DynamicGameObject {
public:
    explicit SideCollider(SideColliderSide side)
            : DynamicGameObject(Vector3(), Vector3(), Vector3(), BOUNDS,  Vector3(), false){
        if (side == LEFT) {
            boundingBox.vecMin = Vector3(-7, -3, -7);
            boundingBox.vecMax = Vector3(-6, 3, -6);
        } else if (side == RIGHT) {
            boundingBox.vecMin = Vector3(7, -3, -6);
            boundingBox.vecMax = Vector3(8, 3, 7);
        }
    }
};

/**/

#endif //AVT7_SIDECOLLIDER_H
