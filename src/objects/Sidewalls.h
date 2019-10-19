//
// Created by skalimat on 19.10.19.
//

#ifndef AVT7_SIDEWALLS_H
#define AVT7_SIDEWALLS_H

#include "../GameObject.h"
#include "../libs/AVTmathLib.h"

class Sidewalls: public GameObject {
public:
    explicit Sidewalls(): GameObject(Vector3()) {

    }

    void init() override {
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_sidewalls);
        createCube(ids.back());

        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_sidewalls);
        createCube(ids.back());

        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_sidewalls);
        createCube(ids.back());

        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_sidewalls);
        createCube(ids.back());
    }

    void render() override {
        pushMatrix(MODEL);
            renderMaterials(ids[0]);
            translate(MODEL, -13 / 2, 0, 7);
            scale(MODEL, 13, 1.2f, 1000);
            buildVAO(ids[0]);
        popMatrix(MODEL);

        pushMatrix(MODEL);
            renderMaterials(ids[1]);
            translate(MODEL, -13 / 2, 0, -1006);
            scale(MODEL, 13, 1.2, 1000);
            buildVAO(ids[1]);
        popMatrix(MODEL);

        pushMatrix(MODEL);
            renderMaterials(ids[2]);
            translate(MODEL, -1006, 0, -1000);
            scale(MODEL, 1000, 2.1, 2000);
            buildVAO(ids[2]);
        popMatrix(MODEL);

        pushMatrix(MODEL);
            renderMaterials(ids[3]);
            translate(MODEL, 7, 0, -1000);
            scale(MODEL, 1000, 2.1, 2000);
            buildVAO(ids[3]);
        popMatrix(MODEL);
    }
};

#endif //AVT7_SIDEWALLS_H
