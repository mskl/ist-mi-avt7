//
// Created by skalimat on 11.10.19.
//

#ifndef AVT7_GAMEMANAGER_H
#define AVT7_GAMEMANAGER_H

#include <math.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <GL/glew.h>     // include GLEW to access OpenGL 3.3 functions
#include <GL/freeglut.h> // GLUT is the toolkit to interface with the OS

// Use Very Simple Libs
#include "libs/AVTmathLib.h"
#include "VertexAttrDef.h"
#include "libs/vsShaderLib.h"
#include "Camera.h"
#include "CameraPerspective.h"
#include "CameraPerspectiveMoving.h"
#include "CameraOrthogonal.h"
#include "objects/River.h"
#include "objects/Ground.h"
#include "objects/Player.h"
#include "objects/Road.h"
#include "objects/Light.h"
#include "objects/Bus.h"
#include "objects/Coordinates.h"
#include "objects/Log.h"
#include "objects/Sidewalls.h"
#include "objects/SideCollider.h"
#include "objects/Target.h"
#include "objects/SpotLight.h"
#include "objects/DirectionalLight.h"
#include "objects/PointLight.h"
#include "objects/Car.h"
#include "objects/Turtle.h"

using namespace std;

const char* VERTEX_SHADER_PATH = "shaders/phong.vert";
const char* FRAGMENT_SHADER_PATH = "shaders/phong.frag";

GLint deltaTime = 1;
GLint prevTime = 1;

GLint pvm_uniformId;
GLint vm_uniformId;
GLint normal_uniformId;

// Light GLSL stuff
GLint l_pos_id[8];  // pointers to shader variables
GLint l_enabled_id; // GLSL pointer to the boolean array
GLint l_enabled[8] = {1, 1, 1, 1, 1, 1, 1, 1};
GLint l_spot_dir_id;

GLint texMode_uniformId;
GLint tex_loc0, tex_loc1, tex_loc2, tex_loc3, tex_loc4;
GLuint TextureArray[25];


class GameManager {
public:
    GLint WindowHandle = 0;
    GLint FrameCount = 0;
    string infoString = "";

    bool isPlaying = true;

    int score = 0;
    int pointsPerTarget = 100;
    float speedMultiplier = 1.2f;

    const float topVerticalLimitPlayerPos = -6.0f;
    const float bottomVerticalLimitPlayerPos = 6.0f;
    const float leftHorizontalLimitPlayerPos = -6.0f;
    const float rightHorizontalLimitPlayerPos = 6.0f;

    vector<GameObject*> gameObjects = vector<GameObject*>();
    vector<Bus*> busses = vector<Bus*>();
    vector<Car*> cars = vector<Car*>();
    vector<Log*> logs = vector<Log*>();
    vector<Turtle*> turtles = vector<Turtle*>();

    // All in microseconds
    GLint lastMoveTime = 0;
    GLint moveTimeout = 300;


    enum CameraType {
        CAMERA_PERSPECTIVE_FOLLOW,
        CAMERA_PERSPECTIVE_FIXED,
        CAMERA_ORTHO
    };

    CameraType currentCameraType = CAMERA_PERSPECTIVE_FOLLOW;
    CameraPerspectiveMoving cameraPerspectiveMoving
            = CameraPerspectiveMoving(20, 0, 40, Vector3(0, 20, 0));
    CameraPerspective cameraPerspectiveFixed
            = CameraPerspective(20, 0, 90, Vector3(0, 20, 0));
    CameraOrthogonal cameraOrthogonal
            = CameraOrthogonal(-7, 8, -8, 7);

    // Player
    Player* player = new Player(Vector3(-5.0f, 1, 0));
    Target* target = new Target(Vector3(0.25f, 1.25f, -5.75f));

    // Lights
    Light* directionalLight = new DirectionalLight(Vector3(0.0f, 5.0f, 0.0f), 6, false);
    SpotLight* spotLight = new SpotLight(Vector3(0, -1, 0), Vector3(0, 2, 0), 7, false);
    vector<PointLight*> pointLights = vector<PointLight*>();

public:
    GameManager() {
        // Keep the pointLight in separate vector as well.
        pointLights.push_back(new PointLight(Vector3(-4.0f, 3.0f, -6.0f), 0, true));
        pointLights.push_back(new PointLight(Vector3(-4.0f, 3.0f, 0.0f), 1, true));
        pointLights.push_back(new PointLight(Vector3(-4.0f, 3.0f, 7.0f), 2, true));
        pointLights.push_back(new PointLight(Vector3(5.0f, 3.0f, -6.0f), 3, true));
        pointLights.push_back(new PointLight(Vector3(5.0f, 3.0f, 0.0f), 4, true));
        pointLights.push_back(new PointLight(Vector3(5.0f, 3.0f, 7.0f), 5, true));

        gameObjects.push_back(new River());
        gameObjects.push_back(new Road());
        gameObjects.push_back(new Ground());
        //gameObjects.push_back(new Sidewalls());

        // The empty side colliders
        gameObjects.push_back(new SideCollider(LEFT));
        gameObjects.push_back(new SideCollider(RIGHT));

        // Save the lights to gameObjects
        for (auto &pl : pointLights)
            gameObjects.push_back(pl);

        gameObjects.push_back(directionalLight);
        gameObjects.push_back(spotLight);

        gameObjects.push_back(player);
        gameObjects.push_back(target);
    }

    void changeSize(int w, int h)
    {
        float ratio;
        // Prevent a divide by zero, when window is too short
        if (h == 0) h = 1;

        // set the viewport to be the entire window
        glViewport(0, 0, w, h);
        loadIdentity(PROJECTION);

        selectCamera(currentCameraType);
    }

    void processKeys(unsigned char key, int xx, int yy)
    {
        switch(key) {
            // Escape exits the game
            case 27: glutLeaveMainLoop(); break;
                // Quality settings
            case 'm': glEnable(GL_MULTISAMPLE); break;
            case 'M': glDisable(GL_MULTISAMPLE); break;
                // Player movement
            case 'p': movePlayer(1, 0); break;
            case 'o': movePlayer(-1, 0); break;
            case 'q': movePlayer(0, -1); break;
            case 'a': movePlayer(0, 1); break;
            // Respawn player
            case 'R': player->respawn(); break;
            // Random target position
            case 'v': target->setRandomPosition(); break;
            // Increase speed
            case 'i': increaseSpeedMultipliers(); break;
            // Toggle the lights
            case 'n': directionalLight->toggleLight(); break;
            case 'h': spotLight->toggleLight(); break;
            case 'c': for (auto &c: pointLights) c->toggleLight(); break;
            case 's':
                if (player->currentLives != 0) {
                    isPlaying = !isPlaying;
                    changeAnimationState(isPlaying);
                } break;
            case 'r':
                if (player->currentLives <= 0) {
                    resetSpeedMultipliers();
                    player->restartGame();
                    isPlaying = true;
                    infoString = "";
                } break;
            // CameraType
            case '1': selectCamera(CAMERA_PERSPECTIVE_FOLLOW); break;
            case '2': selectCamera(CAMERA_PERSPECTIVE_FIXED); break;
            case '3': selectCamera(CAMERA_ORTHO); break;
        }
    }

    void movePlayer(float x, float z) {
        // Check the player movement timeout
        GLint curTime = glutGet(GLUT_ELAPSED_TIME);
        if (curTime - lastMoveTime < moveTimeout) {
            return;
        } else if(!isPlaying) {
            return;
        }

        Vector3 moveVec = Vector3(x, 0, z);
        Vector3 tempPos = player->position + moveVec;
        if (tempPos.z < topVerticalLimitPlayerPos || tempPos.z > bottomVerticalLimitPlayerPos ||
           tempPos.x > rightHorizontalLimitPlayerPos || tempPos.x < leftHorizontalLimitPlayerPos) {
            return;
        }

        lastMoveTime = curTime;
        player->jump(moveVec, 0.5f);
    }

    GLuint setupShaders() {
        // Shader for models
        shader.init();
        shader.loadShader(VSShaderLib::VERTEX_SHADER, VERTEX_SHADER_PATH);
        shader.loadShader(VSShaderLib::FRAGMENT_SHADER, FRAGMENT_SHADER_PATH);

        // set semantics for the shader variables
        // TODO: Use either BindAttrib or GetUniformLocation for all of the GLSL variables
        glBindFragDataLocation(shader.getProgramIndex(), 0,"colorOut");
        glBindAttribLocation(shader.getProgramIndex(), VERTEX_COORD_ATTRIB, "position");
        glBindAttribLocation(shader.getProgramIndex(), NORMAL_ATTRIB, "normal");
        glBindAttribLocation(shader.getProgramIndex(), TEXTURE_COORD_ATTRIB, "texCoord");
        glBindAttribLocation(shader.getProgramIndex(), VERTEX_ATTRIB1, "vVertex");
        glBindAttribLocation(shader.getProgramIndex(), VERTEX_ATTRIB2, "vtexCoord");
        glBindAttribLocation(shader.getProgramIndex(), TANGENT_ATTRIB, "tangent");
        glLinkProgram(shader.getProgramIndex());

        texMode_uniformId = glGetUniformLocation(shader.getProgramIndex(), "texMode");
        tex_loc0 = glGetUniformLocation(shader.getProgramIndex(), "texmap0");

        // Get the indexes of stuff
        pvm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_pvm");
        vm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_viewModel");
        normal_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_normal");

        // Get the light indexes
        for (int i = 0; i < 8; i++) {
            std::string pos = "l_pos_" + to_string(i);
            l_pos_id[i] = glGetUniformLocation(shader.getProgramIndex(), pos.c_str());
        }

        // Get the index of a light boolean mask
        l_enabled_id = glGetUniformLocation(shader.getProgramIndex(), "l_enabled");
        l_spot_dir_id = glGetUniformLocation(shader.getProgramIndex(), "l_spot_dir");

        printf("InfoLog for Per Fragment Phong Lightning Shader\n%s\n\n", shader.getAllInfoLogs().c_str());
        return(shader.isProgramLinked());
    }

    void initScene() {
        srand(time(NULL));

        createVehicles<Bus>(busses, 4, 1, 1, true, 80, 50);
        createVehicles<Car>(cars, 5, 3, 1, false, 80, 50);
        createVehicles<Car>(cars, 5, 5, 1, true, 80, 50);
        createVehicles<Log>(logs, 4, -1, 0, false, 60, 30);
        createVehicles<Log>(logs, 4, -3, 0, false, 60, 30);
        createVehicles<Log>(logs, 4, -5, 0, false, 60, 30);
        createVehicles<Turtle>(turtles, 2, -2, 0, true, 30, 20);
        createVehicles<Turtle>(turtles, 2, -4, 0, true, 30, 20);

        // Initialize all of the GameObjects
        for (GameObject* go : gameObjects) {
            go->init();
        }

        // some GL settings
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_MULTISAMPLE);

        // Sets the background color of the game
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    }

    void renderScene() {
        FrameCount++;
        genDeltaTime();

        // Clear the buffer and load indentity into VIEW and MODEL matrix
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        loadIdentity(VIEW);
        loadIdentity(MODEL);

        if (currentCameraType == CAMERA_PERSPECTIVE_FOLLOW) {
            cameraPerspectiveMoving.pos = player->position;
            cameraPerspectiveMoving.view();
        } else if (currentCameraType == CAMERA_PERSPECTIVE_FIXED) {
            cameraPerspectiveFixed.view();
        } else if (currentCameraType == CAMERA_ORTHO) {
            cameraOrthogonal.view();
        }

        // Use the ShaderLib and print a debug string if needed
        glUseProgram(shader.getProgramIndex());
        if (!shader.isProgramValid()) {
            printf("Program Not Valid!\n");
            exit(1);
        }

        // Update SpotLight position
        spotLight->position = player->position + Vector3(0.5f, 1.2f, 0.5f);
        spotLight->light_dir = Vector3(0, 0, -1);

        // Check for boundaries of the moving objects
        checkVehicleBoundaryCollisions<Bus>(busses);
        checkVehicleBoundaryCollisions<Car>(cars);
        checkVehicleBoundaryCollisions<Log>(logs);
        checkVehicleBoundaryCollisions<Turtle>(turtles);

        // Types of collision determine whether the player has died or not
        bool riverBorder = false;
        bool roadDeath = false;
        bool hitRiver = false;
        bool hitLog = false;

        for (GameObject *go : gameObjects) {
            if (go->isEnabled()) {
                // Update the physics
                if (isPlaying){
                    go->update(deltaTime);
                }

                if (go->position != player->position) {
                    checkPlayerCollisions(go, riverBorder, roadDeath, hitRiver, hitLog);
                }

                // Render the objects ()
                go->render();
            }
        }

        bool deathInRiver = hitRiver && (!hitLog) && (player->playerState != ONLOG);

        if (riverBorder) {
            cout << "Death in river border!" << endl;
            onDeath();
        } else if (deathInRiver) {
            cout << "Death in river!" << endl;
            onDeath();
        } else if (roadDeath) {
            cout << "Death on the road!" << endl;
            onDeath();
        }

        // Swap the back and front buffer
        glutSwapBuffers();
    }

private:
    void checkPlayerCollisions(GameObject *go, bool &riverBorder, bool &roadDeath, bool &hitRiver, bool &hitLog) {
        // Top collisions (bus or car). If riding a turtle or a log, then also check bounds
        if (player->collideWith(go)) {
            if (go->getType() == BUS || go->getType() == CAR) {
                roadDeath = true;
            } else if(go->getType() == TARGET) {
                targetReached();
            } else if (go->getType() == BOUNDS) {
                cout << "Player hit the bounds at " << player->position << endl;
                if (player->playerState == ONLOG || player->playerState == ONTURTLE) {
                    riverBorder = true;
                }
            }
        }

        // Bottom collision
        if (player->collideWithBottom(go)) {
            if (player->playerState == GROUNDED) {
                // Just hit the LOG or TURTLE
                if (go->getType() == LOG) {
                    hitLog = true;
                    player->playerState = ONLOG;
                    player->speed = go->getSpeed();
                } else if (go->getType() == TURTLE) {
                    Turtle *turt = (Turtle *) go;
                    if (!turt->isUnderWater) {
                        hitLog = true;
                        player->playerState = ONTURTLE;
                        player->speed = go->getSpeed();
                    }
                }

                // Is in the river
                if (go->getType() == RIVER) {
                    hitRiver = true;
                }
            } else if (player->playerState == ONTURTLE) {
                // Turtle might get under the water, so we need to check for that.
                // Get the turtle which the player is riding
                if (go->getType() == TURTLE) {
                    Turtle* turt = (Turtle*) go;
                    if(turt->isUnderWater){
                        hitRiver = true;
                        player->playerState = GROUNDED;
                    }
                }
            }
        }
    }

    void targetReached() {
        target->setRandomPosition();
        increaseSpeedMultipliers();
        player->respawn();
        score += pointsPerTarget;
    }

    void onDeath(){
        player->currentLives--;

        if (player->currentLives == 0) {
            infoString = "Player has died! The achieved score: " + to_string(score);
            isPlaying = false;
        }
        player->respawn();
    }

    void genDeltaTime() const {
        GLint currentTime = glutGet(GLUT_ELAPSED_TIME);
        deltaTime = prevTime - currentTime;
        prevTime = currentTime;
    }

    void resetSpeedMultipliers() {
        for (auto &go: gameObjects) {
            go->setSpeedMultiplier(1.0f);
        }
    }

    void increaseSpeedMultipliers() {
        for (auto &go: gameObjects) {
            go->setSpeedMultiplier(go->getSpeedMultiplier()*speedMultiplier);
        }
    }

    void changeAnimationState(bool newState) {
        for (auto &go: gameObjects) {
            go->setAnimationEnabled(newState);
        }
    }

    void selectCamera(CameraType newCamera) {
        currentCameraType = newCamera;

        loadIdentity(PROJECTION);
        if (newCamera == CAMERA_PERSPECTIVE_FOLLOW) {
            cameraPerspectiveMoving.project(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        } else if (newCamera == CAMERA_PERSPECTIVE_FIXED) {
            cameraPerspectiveFixed.project(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        } else if (newCamera == CAMERA_ORTHO) {
            cameraOrthogonal.project(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        }
    }

    template <typename T>
    void createVehicles(vector<T*> &vehicleVector, int n, float zPos, float yPos, bool isGoingRight, int randMod, int randConst) {
        float randSpeed = (float)(rand() % randMod + randConst) / 100.0f;
        for (int j = 0; j < n; j++) {
            int offset = rand() % 7 + 1;

            Vector3 spawnPosition;
            Vector3 spawnSpeed;

            if (isGoingRight){
                spawnPosition = Vector3(-7.0f - offset, yPos, zPos);
                spawnSpeed = Vector3(-randSpeed, 0, 0);
                if (j > 0){
                    spawnPosition.x = vehicleVector.back()->position.x - vehicleVector.back()->boundingBox.vecMax.x - 0.5f - offset;
                }
            } else {
                spawnPosition = Vector3(7.0f + offset, yPos, zPos);
                spawnSpeed = Vector3(randSpeed, 0, 0);
                if (j > 0){
                    spawnPosition.x = vehicleVector.back()->position.x + vehicleVector.back()->boundingBox.vecMax.x  +0.5f + offset;
                }
            }

            T* vehicle = new T(spawnPosition, spawnSpeed, isGoingRight);

            // Add the vehicle to both the GameObjects vector as well as the vehicle-specific vector vehicleVector
            vehicleVector.push_back(vehicle);
            gameObjects.push_back(vehicle);
        }
    }


    template <typename T>
    void checkVehicleBoundaryCollisions(vector<T*> &vehicleVector) {
        const float minX = -6;
        const float maxX = 7;

        for (T* vehicle: vehicleVector) {
            if (vehicle->isGoingRight) {
                if (((vehicle->position.x + vehicle->boundingBox.vecMin.x) > maxX)
                    && ((vehicle->position.x + vehicle->boundingBox.vecMax.x) > maxX)) {
                    vehicle->respawn();
                    int offset = rand() % 3 + 1;
                    vehicle->position.x -= offset;

                    while(checkVehicleCollision(vehicle, vehicleVector)){
                        vehicle->position.x -= (vehicle->boundingBox.vecMax.x + 0.5);
                    }
                }
            } else if (!vehicle->isGoingRight) {
                if (((vehicle->position.x + vehicle->boundingBox.vecMin.x) < minX)
                    && ((vehicle->position.x + vehicle->boundingBox.vecMax.x) < minX)) {
                    vehicle->respawn();
                    int offset = rand() % 3 + 1;
                    vehicle->position.x += offset;
                    while(checkVehicleCollision(vehicle, vehicleVector)){
                        vehicle->position.x += vehicle->boundingBox.vecMax.x + 0.5;
                    }
                }
            }
        }
    }

    template <typename T>
    bool checkVehicleCollision(T* primeVehicle, vector<T*> &vehicleVector){
        for (T* vehicle: vehicleVector) {
            if (vehicle->position != primeVehicle->position) {
                if(primeVehicle->collideWith(vehicle)){
                    return true;
                }
            }
        }
        return false;
    }

    /* TODO: Does not work
    // Check for overlaps in the vehicles
    if (vehicle->position != go->position) {
        if (go->getType() == vehicle->getType()) {
            GameObject *objA = (vehicle->position.x > go->position.x) ? vehicle : go;

            Vector3 tempPos = objA->position;
            if (vehicle->isGoingRight){
                tempPos.x -= (float) (rand() % 5 + 3);
            } else {
                tempPos.x += (float) (rand() % 5 + 3);
            }
            objA->position = tempPos;
        }
    }
    */

};

#endif //AVT7_GAMEMANAGER_H