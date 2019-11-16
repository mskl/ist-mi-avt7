//
// Created by skalimat on 17.10.19.
//

#ifndef AVT7_PLAYER_H
#define AVT7_PLAYER_H

#include "../DynamicGameObject.h"
#include "ParticleSystem.h"

enum PlayerState {JUMPING, GROUNDED, ONLOG, ONTURTLE, JUMPSTAGED};

class Player: public DynamicGameObject {
protected:
    // Position of the start of the jump and the end of the jump
    Vector3 jumpTargetPos = Vector3();
    Vector3 jumpStartPos = Vector3();

    // Calculated position where the jump will happen
    Vector3 transitionPos = Vector3();
    Vector3 transitionJumpDir = Vector3();
    float transitionJumpTime = 0;

    // The bottomBoundingBox check for collisions with river and (or) a log
    BoundingBox bottomBox = BoundingBox(Vector3(0.1, -1, 0.1), Vector3(0.9, 0, 0.9));

public:
    // Current state of the player
    PlayerState playerState = GROUNDED;
    Vector3 lastJumpDir = Vector3();
    Vector3 initPos = Vector3();

    int startingLives = 5;
    int currentLives = startingLives;

    // Particle system assiciated to the player
    ParticleSystem* particleSystem;

    Player(Vector3 pos, ParticleSystem* particleSystem)
        : DynamicGameObject(pos,Vector3(), Vector3(1),PLAYER, Vector3(), false){
        this->particleSystem = particleSystem;
        this->initPos = pos;
    }

    void jump(Vector3 jumpDir, float jumpTime) {
        // Player is riding a log
        if (playerState == ONLOG || playerState == ONTURTLE) {
            playerState = JUMPSTAGED;
            transitionJumpDir = jumpDir;
            transitionJumpTime = jumpTime;

            transitionPos = Vector3();

            if (speed.x > 0) transitionPos.x = floor(position.x);
            else if (speed.x < 0) transitionPos.x = ceil(position.x);
            else transitionPos.x = position.x;

            transitionPos.y = position.y;

            if (speed.z > 0) transitionPos.z = floor(position.z);
            else if (speed.z < 0) transitionPos.z = ceil(position.z);
            else transitionPos.z = position.z;

            /*
            cout << "Player pos: " << position << endl;
            cout << "Player speed: " << speed << endl;
            cout << "Staged transition pos: " << transitionPos << endl;
            */

            lastJumpDir = jumpDir;
        }

        // Player is grounded
        if (playerState == GROUNDED) {
            playerState = JUMPING;
            jumpStartPos = position;
            jumpTargetPos = position + jumpDir;
            speed = (jumpDir * -1) / jumpTime;
            lastJumpDir = jumpDir;
        }
    }

    void update(float deltaTime) final {
        Vector3 deltaSpeed = speed * deltaTime;

        // Player is on a log staged for a jump
        if (playerState == JUMPSTAGED) {
            float targetDistance = transitionPos.distance(position);
            float futureDistance = transitionPos.distance(position + deltaSpeed);

            if (futureDistance > targetDistance) {
                playerState = JUMPING;
                position = transitionPos;
                jumpStartPos = position;
                jumpTargetPos = position + transitionJumpDir;
                speed = (transitionJumpDir * -1) / transitionJumpTime;
            }
        }

        // Player is currently jumping
        else if (playerState == JUMPING) {
            float jumpDistance = jumpStartPos.distance(jumpTargetPos);
            // Measure only flat distance
            float startDistace = jumpStartPos.distance(
                    Vector3(position.x, jumpStartPos.y, position.z));
            float targetDistance = jumpTargetPos.distance(
                    Vector3(position.x, jumpTargetPos.y, position.z));
            float futureDistance = jumpTargetPos.distance(
                    Vector3(position.x, jumpTargetPos.y, position.z) + deltaSpeed);

            // jumpProgress is progress between <0, 1>
            float jumpProgress = (startDistace / jumpDistance);
            position.y = 1 + sin(jumpProgress * 3.14f) / 2.0f;

            if (futureDistance > targetDistance) {
                // Draw a particle system
                Vector3 normalized = speed.normalized();
                particleSystem->spawnParticles(position+Vector3(0.5, 0, 0.5) - normalized / 2.0,
                        speed);

                playerState = GROUNDED;
                speed = Vector3(0, 0, 0);
                position = jumpTargetPos;
            }
        }

        DynamicGameObject::update(deltaTime);
    }

    bool collideWithBottom(const GameObject* other) const {
        return this->bottomBox.isColliding(other->getBoundingBox(), this->position, other->position);
    }

    void respawn(){
        position = initPos;
        playerState = GROUNDED;
        speed = Vector3(0, 0, 0);
    }

    void restartGame() {
        this->respawn();
        this->currentLives = this->startingLives;
    }

    void setSpeedMultiplier(float newSpeed) final {
        // Do nothing on the player
    }

    void init() final {
        // Body
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_player);
        createCube(ids.back());

        // Left Eye
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_player_eye);
        createCube(ids.back());

        // Right Eye
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_player_eye);
        createCube(ids.back());

        // Mouth
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_player_mouth);
        createCube(ids.back());
    }

    void render() final {
        float eyeSize = 0.25;
        float rotation = 0;

        if (lastJumpDir.x == 0 && lastJumpDir.z == 0) rotation = 0;
        else if (lastJumpDir.x == -1 && lastJumpDir.z == 0) rotation = 90;
        else if (lastJumpDir.x == 1 && lastJumpDir.z == 0) rotation = 270;
        else if (lastJumpDir.x == 0 && lastJumpDir.z == 1) rotation = 180;
        else if (lastJumpDir.x == 0 && lastJumpDir.z == -1) rotation = 0;

        renderTexture(texMode_uniformId, 0);
        pushMatrix(MODEL);
            // Body
            renderMaterials(ids[0]);
            translate(MODEL, position.x, position.y, position.z);
            translate(MODEL, 0.1, 0, 0.1);
            scale(MODEL, 0.8, 0.8, 0.8);

            buildVAO(ids[0]);
            // Left eye
            renderMaterials(ids[1]);
            pushMatrix(MODEL);
                translate(MODEL, 0.2, 1, 0);
                scale(MODEL,      0.2, 0.2, 0.2);
                buildVAO(ids[1]);
            popMatrix(MODEL);

            // Right eye
            renderMaterials(ids[2]);
            pushMatrix(MODEL);
                translate(MODEL, 0.6, 1, 0);
                scale(MODEL,0.2, 0.2, 0.2);
                buildVAO(ids[2]);
            popMatrix(MODEL);

            // Mouth
            renderMaterials(ids[3]);
            pushMatrix(MODEL);
                translate(MODEL, 0, 0.2, -0.05f);
                scale(MODEL, 0.8f, 0.25, 0.05f);
                buildVAO(ids[3]);
            popMatrix(MODEL);
        popMatrix(MODEL);
    }
};

#endif //AVT7_PLAYER_H
