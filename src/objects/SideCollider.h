//
// Created by mykha on 22.10.19.
//

#ifndef AVT7_SIDECOLLIDER_H
#define AVT7_SIDECOLLIDER_H

#include <assert.h>
#include "../DynamicGameObject.h"

class SideCollider: public DynamicGameObject {
public:
    explicit SideCollider(Vector3 min, Vector3 max, GameObjectType goType)
            : DynamicGameObject(Vector3(), min, max, goType,  Vector3(), false){
        assert(goType == BOUNDS || goType == DEADLYBOUNDS);
    }

    /*
    void init() override {
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_player_mouth);
        createCubeMinMax(ids.back(), this->boundingBox.vecMin, this->boundingBox.vecMax);
    }

    void render() override {
        pushMatrix(MODEL);
        renderMaterials(ids[0]);
        buildVAO(ids[0]);
        popMatrix(MODEL);
    }
    */
};

/**/

#endif //AVT7_SIDECOLLIDER_H
