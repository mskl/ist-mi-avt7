//
// Created by skalimat on 19.10.19.
//

#ifndef AVT7_LOG_H
#define AVT7_LOG_H

#include "../DynamicGameObject.h"
#include "../libs/AVTmathLib.h"

class Log: public DynamicGameObject {
public:
    // Random offset of the rotation of the log
    float randomTimeOffset = 0.0f;
    // One rotation revolution happens in this time
    const float waveTime = 500.0f;
    float angle = 0;

    GLint prevTime = 1;

    Log(Vector3 pos, Vector3 speed, bool isGoingRight)
            : DynamicGameObject(pos, Vector3(0, 0, 0), Vector3(3, 1, 1), LOG, speed, isGoingRight){
        randomTimeOffset = (((float) rand()) / (float)RAND_MAX) * waveTime;
    }

    void init() override {
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_log);
        createCube(ids.back());
    }

    void render() override {
        renderTexture(texMode_uniformId, 0);
        GLint curTime = glutGet(GLUT_ELAPSED_TIME);
        pushMatrix(MODEL);
            translate(MODEL, position.x, position.y, position.z);
                renderMaterials(ids[0]);
                pushMatrix(MODEL);
                scale(MODEL,     3.0, 0.8, 0.8);
                rotate(MODEL, angle, 1, 0, 0);
                translate(MODEL, 0, 0.2, 0.2);
                buildVAO(ids[0]);
            popMatrix(MODEL);
        popMatrix(MODEL);

    }

    void respawn(){
        position = initPos;
    }

    void update(float deltaTime) override {
        if (animationEnabled) {
            GLint currentTime = glutGet(GLUT_ELAPSED_TIME);
            prevTime = currentTime;
            angle =  8.0f*sin(currentTime/waveTime + randomTimeOffset);
        }

        DynamicGameObject::update(deltaTime);
    }
};

#endif //AVT7_LOG_H
