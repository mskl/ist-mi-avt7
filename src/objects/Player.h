//
// Created by skalimat on 17.10.19.
//

#ifndef AVT7_PLAYER_H
#define AVT7_PLAYER_H

#include "../GameObject.h"


class Player: public GameObject {
public:

    Player(Vector3 pos, int id): GameObject(pos, id) {

    }

    void init() override {
        setMesh(id, customMaterial.amb, customMaterial.diff, customMaterial.spec,
                customMaterial.emissive, customMaterial.shininess, customMaterial.texcount);
        createCube(id);
    }

    void render() override {
        // Set the materials
        renderMaterials(id);

        pushMatrix(MODEL);
        // translate(MODEL, position.x, position.y, position.z);
        buildVAO(id);
        popMatrix(MODEL);
    }
};

#endif //AVT7_PLAYER_H
