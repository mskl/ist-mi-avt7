//
// Created by skalimat on 18.10.19.
//

#ifndef AVT7_TESTCUBECOLLISION_H
#define AVT7_TESTCUBECOLLISION_H

#include "../DynamicGameObject.h"

class TestCubeCollision: public DynamicGameObject {
public:
    float mscale = 0.0f;
    TestCubeCollision(Vector3 pos, Vector3 speed, float tscale, GameObjectType type=TEST,
            Vector3 min = Vector3(0, 0, 0), Vector3 max = Vector3(1, 1, 1))
            : DynamicGameObject(pos, min, max, type, speed) {
        mscale = tscale;
    }

    virtual void respawn(){
        this->position = initPos;
    }

    void init() override {
        // Body
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_bus);
        createCube(ids.back());
    }

    void update(int deltaTime) final {
        DynamicGameObject::update(deltaTime);
    }

    void render() override {
        renderMaterials(ids[0]);
        pushMatrix(MODEL);
            translate(MODEL, position.x, position.y, position.z);
            // The bus body
            pushMatrix(MODEL);
                translate(MODEL, 0, 0, 0);
                scale(MODEL, 1*this->mscale, 1*this->mscale, 1*this->mscale);
                buildVAO(ids[0]);
            popMatrix(MODEL);
        popMatrix(MODEL);
    }

    bool isInside(const GameObject* other) const {
        return this->boundingBox.isInside(other->getBoundingBox(), this->position, other->position);
    }

};

/**/

#endif //AVT7_TESTCUBECOLLISION_H
