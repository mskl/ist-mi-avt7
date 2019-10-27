//
// Created by skalimat on 18.10.19.
//

#ifndef AVT7_DYNAMICGAMEOBJECT_H
#define AVT7_DYNAMICGAMEOBJECT_H

#include "CollidableGameObject.h"

class DynamicGameObject: public CollidableGameObject {
public:
    Vector3 speed;
    Vector3 initSpeed;
    Vector3 initPos;

    // Applies to all of the enviremental moving objects (turtles, logs,..)
    float environmentSpeedMultiplier = 1.0f;


    DynamicGameObject(Vector3 pos, Vector3 min, Vector3 max, GameObjectType type, Vector3 speed)
            : CollidableGameObject(pos, min, max, type), speed(speed) {
        initSpeed = speed;
        initPos = pos;
    }

    Vector3 getSpeed() final {
        return speed * environmentSpeedMultiplier;
    }

    void update(int deltaTime) override {
        position = position + speed*(1.0f/(float)deltaTime)*environmentSpeedMultiplier;
    }

    virtual float getSpeedMultiplier() const {
        return environmentSpeedMultiplier;
    }

    virtual void setSpeedMultiplier(float newSpeedMult) {
       this->environmentSpeedMultiplier = newSpeedMult;
    }
};

#endif //AVT7_DYNAMICGAMEOBJECT_H
