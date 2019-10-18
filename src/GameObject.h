//
// Created by skalimat on 11.10.19.
//

#ifndef AVT7_GAMEOBJECT_H
#define AVT7_GAMEOBJECT_H

extern VSShaderLib shader;
extern GLint pvm_uniformId;
extern GLint vm_uniformId;
extern GLint normal_uniformId;
extern GLint lPos_uniformId;
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
extern float mNormal3x3[9];
extern struct MyMesh mesh[];
extern GLint objId;

#include "Vector3.h"

class GameObject {
public:
    Vector3 position;
    int id;

    GameObject(Vector3 pos, int id) {
        this->position = pos;
        this->id = id;
    }

    virtual void init() { }
    virtual void render() { }
    void update() { }
};

#endif //AVT7_GAMEOBJECT_H
