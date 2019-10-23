//
// Created by skalimat on 18.10.19.
//

#ifndef AVT7_BUS_H
#define AVT7_BUS_H

#include "../DynamicGameObject.h"

class Bus: public DynamicGameObject {
public:

    Vector3 initPos;
    Bus(Vector3 pos, Vector3 speed)
            : DynamicGameObject(pos, Vector3(0, 0, 0), Vector3(3, 1, 1), BUS, speed) {
        initPos = pos;
    }

    void init() override {
        // Body
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_bus);
        createCube(ids.back());

        // Wheel
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_bus_wheel);
        createCube(ids.back());

        // Wheel
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_bus_wheel);
        createCube(ids.back());

        // Wheel
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_bus_wheel);
        createCube(ids.back());

        // Wheel
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_bus_wheel);
        createCube(ids.back());

        // Wheel
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_bus_wheel);
        createCube(ids.back());

        // Wheel
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_bus_wheel);
        createCube(ids.back());
    }

    void render() override {
        float len = 3;

        renderMaterials(ids[0]);
        pushMatrix(MODEL);
            translate(MODEL, position.x, position.y, position.z);
            // The bus body
            pushMatrix(MODEL);
                translate(MODEL, 0, 0.1, 0.1);
                scale(MODEL,     3.0, 0.8, 0.8);
                buildVAO(ids[0]);
            popMatrix(MODEL);

            // Wheel 1
            renderMaterials(ids[1]);
            pushMatrix(MODEL);
                translate(MODEL, 2.5, 0, 0);
                scale(MODEL,     0.3, 0.3, 0.3);
                buildVAO(ids[1]);
            popMatrix(MODEL);

            // Wheel 2
            renderMaterials(ids[2]);
            pushMatrix(MODEL);
                translate(MODEL, 2.5, 0, 1-0.3);
                scale(MODEL,     0.3, 0.3, 0.3);
                buildVAO(ids[2]);
            popMatrix(MODEL);

            // Wheel 11
            renderMaterials(ids[3]);
            pushMatrix(MODEL);
                translate(MODEL, 2.0, 0, 0);
                scale(MODEL,     0.3, 0.3, 0.3);
                buildVAO(ids[4]);
            popMatrix(MODEL);

            // Wheel 22
            renderMaterials(ids[4]);
            pushMatrix(MODEL);
                translate(MODEL, 2.0, 0, 1-0.3);
                scale(MODEL,     0.3, 0.3, 0.3);
                buildVAO(ids[4]);
            popMatrix(MODEL);

            // Wheel 3
            renderMaterials(ids[5]);
            pushMatrix(MODEL);
                translate(MODEL, 0.2, 0, 0);
                scale(MODEL,     0.3, 0.3, 0.3);
                buildVAO(ids[5]);
            popMatrix(MODEL);

            // Wheel 4
            renderMaterials(ids[6]);
            pushMatrix(MODEL);
                translate(MODEL, 0.2, 0, 1-0.3);
                scale(MODEL,     0.3, 0.3, 0.3);
                buildVAO(ids[6]);
            popMatrix(MODEL);
        popMatrix(MODEL);
    }


    void respawn(){
        position = initPos;
    }

};

/**/

#endif //AVT7_BUS_H
