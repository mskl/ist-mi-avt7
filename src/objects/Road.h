//
// Created by skalimat on 17.10.19.
//

#ifndef AVT7_ROAD_H
#define AVT7_ROAD_H

#include "../GameObject.h"

class Road: public GameObject {
public:

    Road(Vector3 pos): GameObject(pos) {

    }

    void init() override {
        ids.push_back(idCount+=1);
        setMesh(ids.back(), customMaterial.amb, customMaterial.diff_black, customMaterial.spec_matte,
                customMaterial.emissive, customMaterial.shininess, customMaterial.texcount);
        createCube(ids.back());
    }

    void render() override {
        renderMaterials(ids[0]);
        pushMatrix(MODEL);
        translate(MODEL, position.x, position.y, position.z);
        translate(MODEL, -13 / 2.0, 0, 0);
        scale(MODEL, 13, 1, 5);
        buildVAO(ids[0]);
        popMatrix(MODEL);
    }
};

/**/

#endif //AVT7_ROAD_H
