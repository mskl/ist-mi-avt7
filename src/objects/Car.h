//
// Created by skalimat on 18.10.19.
//

#ifndef AVT7_CAR_H
#define AVT7_CAR_H

#include "../DynamicGameObject.h"


// TODO: Make bus inherit from car or vice versa, they are the same thing with different body..
class Car: public DynamicGameObject {
public:

    Vector3 initPos;
    bool isGoingRight = false;
    float angle = 0;


    Vector3 initSpeed;
    GLint deltaTime = 1;
    GLint prevTime = 1;

    Car(Vector3 pos, Vector3 speed, bool goingRight)
            : DynamicGameObject(pos, Vector3(0, 0, 0), Vector3(1.5, 1, 1), CAR, speed) {
        initPos = pos;
        isGoingRight = goingRight;
        initSpeed = speed;
    }

    void respawn(){
        position = initPos;
    }

    void init() override {
        // Body
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_car);
        createCube(ids.back());

        // Wheel
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_car_wheel);
        createCube(ids.back());

        // Wheel
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_car_wheel);
        createCube(ids.back());

        // Wheel
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_car_wheel);
        createCube(ids.back());

        // Wheel
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_car_wheel);
        createCube(ids.back());

        // Wheel
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_car_wheel);
        createCube(ids.back());

        // Wheel
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_car_wheel);
        createCube(ids.back());
    }

    void update(int deltaTime) override {
        if (animationEnabled) {
            // Rotate the wheels
            GLint currentTime = glutGet(GLUT_ELAPSED_TIME);
            deltaTime = prevTime - currentTime;
            prevTime = currentTime;

            angle -=50*abs(speed.x)* (1.0f/(float)deltaTime);
        }

        DynamicGameObject::update(deltaTime);
    }

    void render() override {
        renderMaterials(ids[0]);
        pushMatrix(MODEL);
            if (isGoingRight){;
                translate(MODEL, position.x+1.5, position.y, position.z+1);
                rotate(MODEL, 180, 0,1,0);
            }else{
                translate(MODEL, position.x, position.y, position.z);
            }
            // The bus body
            pushMatrix(MODEL);
                translate(MODEL, 0, 0.1, 0.1);
                scale(MODEL,     1.5, 0.5, 0.8);
                buildVAO(ids[0]);
            popMatrix(MODEL);

            // Wheel 1
            renderMaterials(ids[1]);
            pushMatrix(MODEL);
                translate(MODEL, 1.3, 0.1, 0);
                rotate(MODEL, angle, 0,0,1);
                scale(MODEL,     0.3, 0.3, 0.3);
                translate(MODEL, -0.5, -0.5, 0);
                buildVAO(ids[1]);
            popMatrix(MODEL);
            // Wheel 2
            renderMaterials(ids[2]);
            pushMatrix(MODEL);
                translate(MODEL, 1.3, 0.1, 1-0.3);
                rotate(MODEL, angle, 0,0,1);
                scale(MODEL,     0.3, 0.3, 0.3);
                translate(MODEL, -0.5, -0.5, 0);
                buildVAO(ids[2]);
            popMatrix(MODEL);

            // Wheel 3
            renderMaterials(ids[5]);
            pushMatrix(MODEL);
                translate(MODEL, 0.2, 0.1, 0);
                rotate(MODEL, angle, 0,0,1);
                scale(MODEL,     0.3, 0.3, 0.3);
                translate(MODEL, -0.5, -0.5, 0);
                buildVAO(ids[5]);
            popMatrix(MODEL);

            // Wheel 4
            renderMaterials(ids[6]);
            pushMatrix(MODEL);
                translate(MODEL, 0.2, 0.1, 1-0.3);
                rotate(MODEL, angle, 0,0,1);
                scale(MODEL,     0.3, 0.3, 0.3);
                translate(MODEL, -0.5, -0.5, 0);
                buildVAO(ids[6]);
            popMatrix(MODEL);
        popMatrix(MODEL);
    }
};

/**/

#endif //AVT7_CAR_H
