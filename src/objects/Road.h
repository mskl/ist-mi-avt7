//
// Created by skalimat on 17.10.19.
//

#ifndef AVT7_ROAD_H
#define AVT7_ROAD_H

#include "../GameObject.h"

class Road: public GameObject {
public:

    Road(): GameObject(Vector3()) {

    }

    void init() override {
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_road);
        createCube(ids.back());
    }

    void render() override {
        pushMatrix(MODEL);
            pushMatrix(MODEL);
                renderMaterials(ids[0]);
                translate(MODEL, -13 / 2, 0, 1);
                scale(MODEL, 13, 1, 5);
                buildVAO(ids[0]);
            popMatrix(MODEL);
        popMatrix(MODEL);
    }
};

/**/

#endif //AVT7_ROAD_H
