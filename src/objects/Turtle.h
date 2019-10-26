//
// Created by skalimat on 19.10.19.
//

#ifndef AVT7_TURTLE_H
#define AVT7_TURTLE_H

#include "../DynamicGameObject.h"
#include "../libs/AVTmathLib.h"

#include <algorithm>    // std::min

class Turtle: public DynamicGameObject {
public:

    // Initial position of the player
    Vector3 initPos;
    Vector3 initSpeed;
    // Random offset of the rotation of the log
    float randomTimeOffset = 0.0f;
    // One rotation revolution happens in this time
    const float waveTime = 1000.0f;

    GLint prevTime = 1;
    bool isUnderWater = false;
    float posTurtleBodyWater = 0.2f;
    Turtle (Vector3 pos, Vector3 speed)
            : DynamicGameObject(pos, Vector3(0, 0, 0), Vector3(5, 1, 1), TURTLE, speed),
              initPos(pos) {
        randomTimeOffset = (((float) rand()) / (float)RAND_MAX) * waveTime;
        initSpeed = speed;
    }

    void init() override {
        // Body
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_turtle);
        createCube(ids.back());
        // Body
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_turtle);
        createCube(ids.back());
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

        pushMatrix(MODEL);
            translate(MODEL, position.x, position.y, position.z);

            renderMaterials(ids[0]);
            // The turtle body
            pushMatrix(MODEL);
                translate(MODEL, 0, posTurtleBodyWater, 0.1);
                scale(MODEL,     1, 0.8, 0.8);
                buildVAO(ids[0]);
            popMatrix(MODEL);

            // The turtle hump
            pushMatrix(MODEL);
                translate(MODEL, 0, std::min(posTurtleBodyWater*2.2f, posTurtleBodyWater), 0.1);
                scale(MODEL,     0.7, 1.0, 0.8);
                buildVAO(ids[0]);
            popMatrix(MODEL);
            // Head
            pushMatrix(MODEL);
                translate(MODEL, 0.9, std::min(posTurtleBodyWater*2.2f, posTurtleBodyWater), 0.4);
                scale(MODEL,     0.2, 1, 0.25);
                buildVAO(ids[0]);
            popMatrix(MODEL);
            // Top back leg
            pushMatrix(MODEL);
                translate(MODEL, -0.1, posTurtleBodyWater, 0);
                scale(MODEL,     0.3, 0.8, 0.2);
                buildVAO(ids[0]);
            popMatrix(MODEL);

            // Bottom back leg
            pushMatrix(MODEL);
                translate(MODEL, -0.1, posTurtleBodyWater, 0.8);
                scale(MODEL,     0.3, 0.8, 0.2);
                buildVAO(ids[0]);
            popMatrix(MODEL);

            // Top front leg
            pushMatrix(MODEL);
                translate(MODEL, 0.9, posTurtleBodyWater, 0);
                scale(MODEL,     0.3, 0.8, 0.2);
                buildVAO(ids[0]);
            popMatrix(MODEL);
            // Bottom front leg
            pushMatrix(MODEL);
                translate(MODEL, 0.9, posTurtleBodyWater, 0.8);
                scale(MODEL,     0.3, 0.8, 0.2);
                buildVAO(ids[0]);
            popMatrix(MODEL);


            // The turtle body
            pushMatrix(MODEL);
                translate(MODEL, 1.8, posTurtleBodyWater, 0.1);
                scale(MODEL,     1, 0.8, 0.8);
                buildVAO(ids[0]);
            popMatrix(MODEL);

            // The turtle hump
            pushMatrix(MODEL);
                translate(MODEL, 1.8, std::min(posTurtleBodyWater*2.2f, posTurtleBodyWater), 0.1);
                scale(MODEL,     0.7, 1.0, 0.8);
                buildVAO(ids[0]);
            popMatrix(MODEL);
            // Head
            pushMatrix(MODEL);
                translate(MODEL, 2.7, std::min(posTurtleBodyWater*2.2f, posTurtleBodyWater), 0.4);
                scale(MODEL,     0.2, 1, 0.25);
                buildVAO(ids[0]);
            popMatrix(MODEL);
            // Top back leg
            pushMatrix(MODEL);
                translate(MODEL, 1.7, posTurtleBodyWater, 0);
                scale(MODEL,     0.3, 0.8, 0.2);
                buildVAO(ids[0]);
            popMatrix(MODEL);

            // Bottom back leg
            pushMatrix(MODEL);
                translate(MODEL, 1.7, posTurtleBodyWater, 0.8);
                scale(MODEL,     0.3, 0.8, 0.2);
                buildVAO(ids[0]);
            popMatrix(MODEL);

            // Top front leg
            pushMatrix(MODEL);
                translate(MODEL, 2.7, posTurtleBodyWater, 0);
                scale(MODEL,     0.3, 0.8, 0.2);
                buildVAO(ids[0]);
            popMatrix(MODEL);
            // Bottom front leg
            pushMatrix(MODEL);
                translate(MODEL, 2.7, posTurtleBodyWater, 0.8);
                scale(MODEL,     0.3, 0.8, 0.2);
                buildVAO(ids[0]);
            popMatrix(MODEL);

            // The turtle body
            pushMatrix(MODEL);
                translate(MODEL, 3.6, posTurtleBodyWater, 0.1);
                scale(MODEL,     1, 0.8, 0.8);
                buildVAO(ids[0]);
            popMatrix(MODEL);

            // The turtle hump
            pushMatrix(MODEL);
                translate(MODEL, 3.6, std::min(posTurtleBodyWater*2.2f, posTurtleBodyWater), 0.1);
                scale(MODEL,     0.7, 1.0, 0.8);
                buildVAO(ids[0]);
            popMatrix(MODEL);
            // Head
            pushMatrix(MODEL);
                translate(MODEL, 4.5, std::min(posTurtleBodyWater*2.2f, posTurtleBodyWater), 0.4);
                scale(MODEL,     0.2, 1, 0.25);
                buildVAO(ids[0]);
            popMatrix(MODEL);
            // Top back leg
            pushMatrix(MODEL);
                translate(MODEL, 3.5, posTurtleBodyWater, 0);
                scale(MODEL,     0.3, 0.8, 0.2);
                buildVAO(ids[0]);
            popMatrix(MODEL);

            // Bottom back leg
            pushMatrix(MODEL);
                translate(MODEL, 3.5, posTurtleBodyWater, 0.8);
                scale(MODEL,     0.3, 0.8, 0.2);
                buildVAO(ids[0]);
            popMatrix(MODEL);

            // Top front leg
            pushMatrix(MODEL);
                translate(MODEL, 4.5, posTurtleBodyWater, 0);
                scale(MODEL,     0.3, 0.8, 0.2);
                buildVAO(ids[0]);
            popMatrix(MODEL);
            // Bottom front leg
            pushMatrix(MODEL);
                translate(MODEL, 4.5, posTurtleBodyWater, 0.8);
                scale(MODEL,     0.3, 0.8, 0.2);
                buildVAO(ids[0]);
            popMatrix(MODEL);
        popMatrix(MODEL);

    }

    void respawn(){
        position = initPos;
    }

    void moveTurtle(){
        GLint currentTime = glutGet(GLUT_ELAPSED_TIME);
        GLint deltaTime = prevTime - currentTime;
        prevTime = currentTime;
        posTurtleBodyWater =  0.2f*sin(currentTime/waveTime + randomTimeOffset);
        isUnderWater = posTurtleBodyWater < 0.0f;
    }
};

#endif //AVT7_TURTLE_H
