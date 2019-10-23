//
// Created by Mika412 on 23.10.19.
//

#ifndef AVT7_TARGET_H
#define AVT7_TARGET_H

#include "../DynamicGameObject.h"

class Target: public DynamicGameObject {
public:

    float angle = 0;
    Target(Vector3 pos)
            : DynamicGameObject(pos, Vector3(0, 0, 0), Vector3(1, 1, 1), TARGET, Vector3()) {
    }

    void init() override {
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_target);
        createCube(ids.back());
    }

    void render() override {
        float len = 3;

        renderMaterials(ids[0]);
        pushMatrix(MODEL);
            translate(MODEL, position.x, position.y, position.z);
            rotate(MODEL, angle, 1,1,1);
            pushMatrix(MODEL);
                scale(MODEL,     0.5f, 0.5f, 0.5f);
                buildVAO(ids[0]);
            popMatrix(MODEL);
        popMatrix(MODEL);
    }


    void rotateCube(int deltaTime) {
        angle +=50* (1.0f/(float)deltaTime);
    }

};

/**/

#endif //AVT7_TARGET_H
