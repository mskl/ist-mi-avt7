//
// Created by mykha on 5.11.19.
//

#ifndef AVT7_RIVERBLOCK_H
#define AVT7_RIVERBLOCK_H

#include "../CollidableGameObject.h"

extern GLint texMode_uniformId;
class RiverBlock: public CollidableGameObject {
public:
    RiverBlock(Vector3 pos): CollidableGameObject(pos, Vector3(0, 0, 0), Vector3(1, 0.8, 1), RIVER) {

    }

    void init() {
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_river);
        createCube(ids.back());

    }

    void render() {
        pushMatrix(MODEL);
            renderMaterials(ids[0]);
            pushMatrix(MODEL);
                translate(MODEL, position.x, position.y, position.z);
                //renderTexture(texMode_uniformId, 2);
                scale(MODEL, 1, 0.8, 1);
                buildVAO(ids[0], true);

            popMatrix(MODEL);

        popMatrix(MODEL);
    }
};

/**/
#endif //AVT7_RIVERBLOCK_H
