//
// Created by skalimat on 17.10.19.
//

#ifndef AVT7_PLAYER_H
#define AVT7_PLAYER_H

#include "../DynamicGameObject.h"


enum PlayerState {GROUNDED, JUMPING, ONLOG};

class Player: public DynamicGameObject {
protected:
    PlayerState playerState = GROUNDED;
    Vector3 jumpStart = Vector3();
    Vector3 jumpTarget = Vector3();

public:

    Player(Vector3 pos)
        : DynamicGameObject(pos,Vector3(), Vector3(1),PLAYER, Vector3()) {
    }

    void jump(Vector3 target, float jumpSpeed) {
        playerState = JUMPING;
        jumpStart = position;
        jumpTarget = target;
        speed = (jumpStart - jumpTarget) / jumpSpeed;
    }

    void init() final {
        // Body
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_player);
        createCube(ids.back());

        // Left Eye
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_player);
        createCube(ids.back());

        // Right Eye
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_player);
        createCube(ids.back());

        // Mouth
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_player);
        createCube(ids.back());
    }

    void render() final {
        float eyeSize = 0.25;

        // Body
        renderMaterials(ids[0]);
        pushMatrix(MODEL);
            translate(MODEL, -0.5, 0, -0.5);
            translate(MODEL, position.x, position.y, position.z);
            buildVAO(ids[0]);

            renderMaterials(ids[1]);
            pushMatrix(MODEL);
                translate(MODEL, -0.25, 0.75,  -0.25);
                scale(MODEL,      0.5, 0.5, 0.5);
                buildVAO(ids[1]);
            popMatrix(MODEL);

            renderMaterials(ids[2]);
            pushMatrix(MODEL);
                pushMatrix(MODEL);
                translate(MODEL, 0.75, 0.75,  -0.25);
                scale(MODEL,      0.5, 0.5, 0.5);
                buildVAO(ids[2]);
            popMatrix(MODEL);

            renderMaterials(ids[3]);
            pushMatrix(MODEL);
                translate(MODEL, 0, 0.2, -0.025);
                scale(MODEL, 0.8f, 0.25, 0.05f);
                buildVAO(ids[3]);
            popMatrix(MODEL);
        popMatrix(MODEL);

    }

    void update(int deltaTime) final {
        // Update the position
        DynamicGameObject::update(deltaTime);

        // If jumping
        if (playerState == JUMPING) {
            if(position.distance(jumpStart) > jumpStart.distance(jumpTarget)){
                playerState = GROUNDED;
                speed = Vector3(0, 0, 0);
            }
        }
    }
};

#endif //AVT7_PLAYER_H
