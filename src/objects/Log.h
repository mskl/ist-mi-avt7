//
// Created by skalimat on 19.10.19.
//

#ifndef AVT7_LOG_H
#define AVT7_LOG_H

#include "../DynamicGameObject.h"
#include "../libs/AVTmathLib.h"

class Log: public DynamicGameObject {
public:
    Log(Vector3 pos, Vector3 speed)
            : DynamicGameObject(pos, Vector3(0, 0, 0), Vector3(3, 1, 1), LOG, speed) {
    }

    void init() override {
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_log);
        createCube(ids.back());
    }

    void render() override {
        pushMatrix(MODEL);
            translate(MODEL, position.x, position.y, position.z);
                renderMaterials(ids[0]);
                pushMatrix(MODEL);
                scale(MODEL,     3.0, 0.8, 0.8);
                translate(MODEL, 0, 0.2, 0.2);
                buildVAO(ids[0]);
            popMatrix(MODEL);
        popMatrix(MODEL);
    }
};

#endif //AVT7_LOG_H
