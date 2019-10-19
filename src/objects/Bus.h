//
// Created by skalimat on 18.10.19.
//

#ifndef AVT7_BUS_H
#define AVT7_BUS_H

#include "../DynamicGameObject.h"

class Bus: public DynamicGameObject {
public:
    Bus(Vector3 pos, Vector3 speed)
            : DynamicGameObject(pos, Vector3(0, 0, 0), Vector3(3, 1, 1), BUS, speed) {
    }

    void init() override {
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_bus);
        createCube(ids.back());
    }

    void render() override {
        float len = 3;

        renderMaterials(ids[0]);
        pushMatrix(MODEL);
            translate(MODEL, position.x, position.y, position.z);
            pushMatrix(MODEL);
                scale(MODEL,     3.0, 1, 1);
                buildVAO(ids[0]);
            popMatrix(MODEL);
        popMatrix(MODEL);
    }
};

/**/

#endif //AVT7_BUS_H
