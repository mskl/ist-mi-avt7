//
// Created by skalimat on 19.10.19.
//

#ifndef AVT7_TURTLE_H
#define AVT7_TURTLE_H

#include "../DynamicGameObject.h"
#include "../libs/AVTmathLib.h"

class Turtle: public DynamicGameObject {
public:

    // Initial position of the player
    Vector3 initPos;
    // Random offset of the rotation of the log
    float randomTimeOffset = 0.0f;
    // One rotation revolution happens in this time
    const float waveTime = 1000.0f;
    bool isUnderWater = false;
    Turtle(Vector3 pos, Vector3 speed)
            : DynamicGameObject(pos, Vector3(0, 0, 0), Vector3(5, 1, 1), TURTLE, speed),
              initPos(pos) {
        randomTimeOffset = (((float) rand()) / (float)RAND_MAX) * waveTime;
    }

    void init() override {
        // Body
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_turtle);
        createCube(ids.back());

        // Body
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_turtle_dark);
        createCube(ids.back());

        // Body
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_turtle);
        createCube(ids.back());
    }

    void render() override {
        GLint curTime = glutGet(GLUT_ELAPSED_TIME);

        float posTurtleBodyWater = 0.2f*sin(curTime/(waveTime +randomTimeOffset));

        isUnderWater = posTurtleBodyWater < 0.0f;

        pushMatrix(MODEL);
            translate(MODEL, position.x, position.y, position.z);

            renderMaterials(ids[0]);
            // The bus body
            pushMatrix(MODEL);
                translate(MODEL, 0, posTurtleBodyWater, 0.1);
                scale(MODEL,     1.5, 0.8, 0.8);
                buildVAO(ids[0]);
            popMatrix(MODEL);
            pushMatrix(MODEL);
                translate(MODEL, 1.8, posTurtleBodyWater, 0.1);
                scale(MODEL,     1.5, 0.8, 0.8);
                buildVAO(ids[1]);
            popMatrix(MODEL);
            pushMatrix(MODEL);
            translate(MODEL, 3.6, posTurtleBodyWater, 0.1);
                scale(MODEL,     1.5, 0.8, 0.8);
                buildVAO(ids[2]);
            popMatrix(MODEL);
        popMatrix(MODEL);

    }

    void respawn(){
        position = initPos;
    }
};

#endif //AVT7_TURTLE_H
