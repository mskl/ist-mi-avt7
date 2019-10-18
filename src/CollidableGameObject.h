//
// Created by skalimat on 18.10.19.
//

#ifndef AVT7_COLLIDABLEGAMEOBJECT_H
#define AVT7_COLLIDABLEGAMEOBJECT_H

#include "GameObject.h"
#include "BoundingBox.h"

enum GameObjectType {BOUNDS, TARGET, GRASS, LOG, RIVER, GROUND, BUS, ROAD, UNKNOWN};

class CollidableGameObject: public GameObject {
public:
    BoundingBox boundingBox;
    GameObjectType gameObjectType;

    CollidableGameObject(Vector3 pos, int id, Vector3 min, Vector3 max, GameObjectType type = UNKNOWN)
        : GameObject(pos, id), boundingBox(min, max), gameObjectType(type) { }
};


#endif //AVT7_COLLIDABLEGAMEOBJECT_H
