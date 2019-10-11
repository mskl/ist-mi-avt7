//
// Created by skalimat on 11.10.19.
//

#ifndef AVT7_GAMEOBJECT_H
#define AVT7_GAMEOBJECT_H

#include "Vector3.h"

class GameObject
    {
public:
    GameObject();

    Vector3* getPosition();
    void setPosition(float _x, float _y, float _z) {

    }
    void setPosition(Vector3 _pos) {

    }

protected:
    Vector3 _position;
};

#endif //AVT7_GAMEOBJECT_H
