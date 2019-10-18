//
// Created by skalimat on 17.10.19.
//

#ifndef AVT7_PLAYER_H
#define AVT7_PLAYER_H

#include "../CollidableGameObject.h"


class Player: public CollidableGameObject {
public:

    Player(Vector3 pos)
        : CollidableGameObject(pos, Vector3(0, 0, 0), Vector3(1, 1, 1), PLAYER) {

    }

    void init() override {
        // Body
        ids.push_back(idCount+=1);
        setMesh(ids.back(), customMaterial.amb, customMaterial.diff, customMaterial.spec,
                customMaterial.emissive, customMaterial.shininess, customMaterial.texcount);
        createCube(ids.back());

        // Left Eye
        ids.push_back(idCount+=1);
        setMesh(ids.back(), customMaterial.amb, customMaterial.diff_yellow, customMaterial.spec,
                customMaterial.emissive, customMaterial.shininess, customMaterial.texcount);
        createCube(ids.back());

        // Right Eye
        ids.push_back(idCount+=1);
        setMesh(ids.back(), customMaterial.amb, customMaterial.diff_yellow, customMaterial.spec,
                customMaterial.emissive, customMaterial.shininess, customMaterial.texcount);
        createCube(ids.back());

        // Mouth
        ids.push_back(idCount+=1);
        setMesh(ids.back(), customMaterial.amb, customMaterial.diff_red, customMaterial.spec,
                customMaterial.emissive, customMaterial.shininess, customMaterial.texcount);
        createCube(ids.back());
    }

    void render() override {
        float eyeSize = 0.25;

        // Body
        renderMaterials(ids[0]);
        pushMatrix(MODEL);
            translate(MODEL, -0.5, 0, -0.5);
            translate(MODEL, position.x, position.y, position.z);
            buildVAO(ids[0]);

            renderMaterials(ids[1]);
            pushMatrix(MODEL);
                translate(MODEL, -0.25, 0.75,  -0.25);
                scale(MODEL,      0.5, 0.5, 0.5);
                buildVAO(ids[1]);
            popMatrix(MODEL);

            renderMaterials(ids[2]);
            pushMatrix(MODEL);
                pushMatrix(MODEL);
                translate(MODEL, 0.75, 0.75,  -0.25);
                scale(MODEL,      0.5, 0.5, 0.5);
                buildVAO(ids[2]);
            popMatrix(MODEL);

            renderMaterials(ids[3]);
            pushMatrix(MODEL);
                translate(MODEL, 0, 0.2, -0.025);
                scale(MODEL, 0.8f, 0.25, 0.05f);
                buildVAO(ids[3]);
            popMatrix(MODEL);
        popMatrix(MODEL);

    }
};

#endif //AVT7_PLAYER_H
