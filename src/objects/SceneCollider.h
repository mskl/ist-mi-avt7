//
// Created by mykha on 22.10.19.
//

#ifndef AVT7_SCENECOLLIDER_H
#define AVT7_SCENECOLLIDER_H

#include "../DynamicGameObject.h"

class SceneCollider: public DynamicGameObject {
public:
    explicit SceneCollider(Vector3 pos)
            : DynamicGameObject(pos, Vector3(0, 0, 0), Vector3(13.0, 5, 13), BOUNDS,  Vector3(0, 0, 0), false){
    }

    void init() override {
        ids.push_back(idCount+=1);
        //setMaterial(ids.back(), mat_bus);
        //createCube(ids.back());
    }

    void render() override {
        float len = 3;

        renderMaterials(ids[0]);
        pushMatrix(MODEL);
            translate(MODEL, position.x, position.y, position.z);
            pushMatrix(MODEL);
                scale(MODEL,     13.0, 5, 13);
                buildVAO(ids[0]);
            popMatrix(MODEL);
        popMatrix(MODEL);
    }
};

/**/

#endif //AVT7_SCENECOLLIDER_H
