//
// Created by skalimat on 17.10.19.
//

#ifndef AVT7_RIVER_H
#define AVT7_RIVER_H

#include "../CollidableGameObject.h"

extern GLint texMode_uniformId;
class River: public CollidableGameObject {
public:
    River(): CollidableGameObject(Vector3(), Vector3(-6, 0, -6), Vector3(7, 0.8, 0), RIVER) {

    }

    void init() {
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_river);
        createCube(ids.back());

        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_river_bottom);
        createCube(ids.back());
        isTransparent = true;
    }

    void render() {
        pushMatrix(MODEL);
            renderMaterials(ids[0]);
            pushMatrix(MODEL);
                translate(MODEL, -13 / 2, 0, -6);
                //renderTexture(texMode_uniformId, 2);
                scale(MODEL, 13, 0.8, 6);
                buildVAO(ids[0], true);

            popMatrix(MODEL);

            renderMaterials(ids[1]);
            pushMatrix(MODEL);
                translate(MODEL, -13 / 2, 0, -6);
                //renderTexture(texMode_uniformId, 2);
                scale(MODEL, 13, 0.1, 6);
                buildVAO(ids[1], true);
            popMatrix(MODEL);
        popMatrix(MODEL);
    }
};

/**/
#endif //AVT7_RIVER_H
