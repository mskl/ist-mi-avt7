//
// Created by skalimat on 18.10.19.
//

#ifndef AVT7_CAR_H
#define AVT7_CAR_H

#include "Bus.h"

class Car: public Bus {
public:
    Car (Vector3 pos, Vector3 speed, bool goingRight)
            : Bus(pos, speed, goingRight, CAR, Vector3(0, 0, 0), Vector3(1.5, 1, 1)) {
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

    void render() override {
        renderMaterials(ids[0]);
        renderTexture(texMode_uniformId, 0);

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
