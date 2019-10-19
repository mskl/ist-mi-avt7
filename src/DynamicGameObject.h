//
// Created by skalimat on 18.10.19.
//

#ifndef AVT7_DYNAMICGAMEOBJECT_H
#define AVT7_DYNAMICGAMEOBJECT_H

#include "CollidableGameObject.h"

class DynamicGameObject: public CollidableGameObject {
public:
    Vector3 speed;

    DynamicGameObject(Vector3 pos, Vector3 min, Vector3 max, GameObjectType type, Vector3 speed)
            : CollidableGameObject(pos, min, max, type), speed(speed) {

    }

    Vector3 getSpeed() final {
        return speed;
    }

    void update(int deltaTime) override {
        position = position + speed*(1.0f/(float)deltaTime);
    }
};

#endif //AVT7_DYNAMICGAMEOBJECT_H
