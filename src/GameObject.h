//
// Created by skalimat on 11.10.19.
//

#ifndef AVT7_GAMEOBJECT_H
#define AVT7_GAMEOBJECT_H

#include "Vector3.h"

class GameObject
    {
public:
    GameObject() = default;

    Vector3 getPosition() {
        return this->_position;
    }

    void setPosition(float _x, float _y, float _z) {
        this->_position.set(_x, _y, _z);
    }

    void setPosition(Vector3 _pos) {
        this->_position = _pos;
    }

protected:
    Vector3 _position;
};

#endif //AVT7_GAMEOBJECT_H
