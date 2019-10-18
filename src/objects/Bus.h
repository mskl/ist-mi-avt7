//
// Created by skalimat on 18.10.19.
//

#ifndef AVT7_BUS_H
#define AVT7_BUS_H

#include "../DynamicGameObject.h"

class Bus: public DynamicGameObject {
public:
    Bus(Vector3 pos, Vector3 speed)
            : DynamicGameObject(pos, Vector3(0, 0, 0), Vector3(5, 1, 1), BUS, speed) {
    }

    void init() override {
        ids.push_back(idCount+=1);
        setMesh(ids.back(), customMaterial.amb, customMaterial.diff_red, customMaterial.spec,
                customMaterial.emissive, customMaterial.shininess, customMaterial.texcount);
        createCube(ids.back());
    }

    void render() override {
        float len = 3;

        renderMaterials(ids[0]);
        pushMatrix(MODEL);
        scale(MODEL,      3, 1, 1);
        translate(MODEL, 0.5/3.0, 0,  -0.5);
        translate(MODEL, position.x/3, position.y, position.z);
        buildVAO(ids[0]);
        popMatrix(MODEL);
    }
};

/**/

#endif //AVT7_BUS_H
