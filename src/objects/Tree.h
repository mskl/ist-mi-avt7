//
// Created by skalimat on 13.11.19.
//

#ifndef AVT7_TREE_H
#define AVT7_TREE_H

#include "../GameObject.h"
#include "../libs/Billboard.h"

class Tree: public GameObject {
public:
    Tree(Vector3 pos): GameObject(pos) {

    }

    void init() override {
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_ground);
        createQuad(ids.back(), 6, 6);
    }

    void render(Vector3 camPos) {
        renderTexture(texMode_uniformId, 4);
        float* cam = new float[3];
        float* pos = new float[3];

        pos[0] = position.x;
        pos[1] = position.y;
        pos[2] = position.z;

        cam[0] = camPos.x;
        // do not rotate around Y axis
        cam[1] = 1; // camPos.y
        cam[2] = camPos.z;

        pushMatrix(MODEL);
            renderMaterials(ids[0]);
            translate(MODEL, position.x, position.y, position.z);
            l3dBillboardSphericalBegin(cam, pos);
            buildVAO(ids[0]);
        popMatrix(MODEL);

        delete[] cam;
        delete[] pos;
    }
};

#endif //AVT7_TREE_H
