//
// Created by skalimat on 17.10.19.
//

#ifndef AVT7_PLAYER_H
#define AVT7_PLAYER_H

#include "../GameObject.h"


class Player: public GameObject {
public:

    Player(Vector3 pos): GameObject(pos) {

    }

    void init() override {
        ids.push_back(idCount+=1);
        setMesh(ids.back(), customMaterial.amb, customMaterial.diff, customMaterial.spec,
                customMaterial.emissive, customMaterial.shininess, customMaterial.texcount);
        createCube(ids.back());
    }

    void render() override {
        renderMaterials(ids[0]);
        pushMatrix(MODEL);
        translate(MODEL, -0.5, 0, -0.5);
        translate(MODEL, position.x, position.y, position.z);
        buildVAO(ids[0]);
        popMatrix(MODEL);
    }
};

#endif //AVT7_PLAYER_H
