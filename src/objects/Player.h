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
        // Body
        renderMaterials(ids[0]);
        pushMatrix(MODEL);
        translate(MODEL, -0.5, 0, -0.5);
        translate(MODEL, position.x, position.y, position.z);
        buildVAO(ids[0]);
        popMatrix(MODEL);

        float eyeSize = 0.3;

        // LeftEye
        renderMaterials(ids[1]);
        pushMatrix(MODEL);
        translate(MODEL, -eyeSize, 0, -eyeSize/2.0f);
        translate(MODEL, 0.5, 1-eyeSize, 0.65f);
        translate(MODEL, position.x, position.y, position.z);    // Move with player
        scale(MODEL, eyeSize, eyeSize, 0.05);
        buildVAO(ids[1]);
        popMatrix(MODEL);

        // RightEye
        renderMaterials(ids[2]);
        pushMatrix(MODEL);
        translate(MODEL, eyeSize, 0, -eyeSize/2.0f);
        translate(MODEL, -0.5, 1-eyeSize, 0.65f);
        translate(MODEL, position.x, position.y, position.z);    // Move with player
        scale(MODEL, eyeSize, eyeSize, 0.05);
        buildVAO(ids[2]);
        popMatrix(MODEL);

        // Mouth
        renderMaterials(ids[3]);
        pushMatrix(MODEL);
        translate(MODEL, eyeSize, 0, -eyeSize/2.0f);
        translate(MODEL, -0.6, 0.2, 0.65f);
        translate(MODEL, position.x, position.y, position.z);    // Move with player
        scale(MODEL, 0.8f, 0.25, 0.05f);
        buildVAO(ids[3]);
        popMatrix(MODEL);

    }
};

#endif //AVT7_PLAYER_H
