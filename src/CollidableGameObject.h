//
// Created by skalimat on 18.10.19.
//

#ifndef AVT7_COLLIDABLEGAMEOBJECT_H
#define AVT7_COLLIDABLEGAMEOBJECT_H

#include "GameObject.h"
#include "BoundingBox.h"

class CollidableGameObject: public GameObject {
public:
    BoundingBox boundingBox;
    GameObjectType gameObjectType;

    CollidableGameObject(Vector3 pos, Vector3 min, Vector3 max, GameObjectType type = UNKNOWN)
        : GameObject(pos), boundingBox(min, max), gameObjectType(type) {

    }

    virtual GameObjectType getType() override {
        return gameObjectType;
    }

    virtual BoundingBox getBoundingBox() const override {
        return this->boundingBox;
    }

    virtual bool collideWith(const GameObject* other) const override {
        return this->boundingBox.isColliding(other->getBoundingBox(), position, other->position);
    }
    virtual bool isInsideOther(const GameObject* other) const {
        return this->boundingBox.isInside(other->getBoundingBox());
    }
};


#endif //AVT7_COLLIDABLEGAMEOBJECT_H
