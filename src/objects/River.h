//
// Created by skalimat on 17.10.19.
//

#ifndef AVT7_RIVER_H
#define AVT7_RIVER_H

#include "../CollidableGameObject.h"

extern GLint texMode_uniformId;
class River: public CollidableGameObject {
public:
    River(): CollidableGameObject(Vector3(), Vector3(-6, 0, -5), Vector3(7, 0.8, 0), RIVER) {

    }

    void init() {
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_river);
        createCube(ids.back());
    }

    void render() {
        pushMatrix(MODEL);
            pushMatrix(MODEL);
                renderMaterials(ids[0]);
                translate(MODEL, -13 / 2, 0, -5);
                renderTexture(texMode_uniformId, 2);
                scale(MODEL, 13, 0.8, 5);
                buildVAO(ids[0]);
            popMatrix(MODEL);
        popMatrix(MODEL);
    }
};

/**/
#endif //AVT7_RIVER_H
