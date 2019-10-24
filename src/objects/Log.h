//
// Created by skalimat on 19.10.19.
//

#ifndef AVT7_LOG_H
#define AVT7_LOG_H

#include "../DynamicGameObject.h"
#include "../libs/AVTmathLib.h"

class Log: public DynamicGameObject {
public:

    // Initial position of the player
    Vector3 initPos;
    // Random offset of the rotation of the log
    float randomTimeOffset = 0.0f;
    // One rotation revolution happens in this time
    const float waveTime = 500.0f;

    Vector3 initSpeed;
    Log(Vector3 pos, Vector3 speed)
            : DynamicGameObject(pos, Vector3(0, 0, 0), Vector3(3, 1, 1), LOG, speed),
              initPos(pos) {
        randomTimeOffset = (((float) rand()) / (float)RAND_MAX) * waveTime;
        initSpeed = speed;
    }

    void init() override {
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_log);
        createCube(ids.back());
    }

    void render() override {
        GLint curTime = glutGet(GLUT_ELAPSED_TIME);
        pushMatrix(MODEL);
            translate(MODEL, position.x, position.y, position.z);
                renderMaterials(ids[0]);
                pushMatrix(MODEL);
                scale(MODEL,     3.0, 0.8, 0.8);
                rotate(MODEL, 8.0f*sin(curTime/waveTime + randomTimeOffset), 1, 0, 0);
                translate(MODEL, 0, 0.2, 0.2);
                buildVAO(ids[0]);
            popMatrix(MODEL);
        popMatrix(MODEL);

    }

    void respawn(){
        position = initPos;
    }
};

#endif //AVT7_LOG_H
