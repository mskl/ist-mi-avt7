//
// Created by skalimat on 11.10.19.
//

#ifndef AVT7_GAMEMANAGER_H
#define AVT7_GAMEMANAGER_H

#include <math.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include "stdio.h"

using namespace std;

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


const char* VERTEX_SHADER_PATH = "shaders/pointlight.vert";
const char* FRAGMENT_SHADER_PATH = "shaders/pointlight.frag";

GLint deltaTime = 1;
GLint prevTime = 1;

GLint pvm_uniformId;
GLint vm_uniformId;
GLint normal_uniformId;
GLint lPos_uniformId;

class GameManager {
public:
    int WindowHandle = 0;
    unsigned int FrameCount = 0;

    bool isPlaying = true;

    float topVerticalLimitPlayerPos = -6.0f;
    float bottomVerticalLimitPlayerPos = 6.0f;
    float leftHorizontalLimitPlayerPos = -6.0f;
    float rightHorizontalLimitPlayerPos = 6.0f;

    std::vector<GameObject*> gameObjects = std::vector<GameObject*>();

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
        = CameraOrthogonal(-15, 15, -15, 15,Vector3(0, 20, 0));

    Player* player = new Player(Vector3(0, 1, 0));
    Light* directionalLight = new Light(Vector3(0.0f, -0.1f, 0.0f), 0);
    Bus* bus = new Bus(Vector3(6, 1, 1), Vector3(1, 0, 0));
    Log* log = new Log(Vector3(6, 0, -3), Vector3(0.5f, 0, 0));

    Coordinates* cmin = new Coordinates(Vector3(-6, 0, -5));
    Coordinates* cmax = new Coordinates(Vector3(7, 0.8, 0));

public:
    GameManager() {
        gameObjects.push_back(new River());
        gameObjects.push_back(new Road());
        gameObjects.push_back(new Ground());
        gameObjects.push_back(new Sidewalls());
        gameObjects.push_back(new Light(Vector3(4.0f, 6.0f, 2.0f), 1));
        gameObjects.push_back(new Log(Vector3(7, 0, -1), Vector3(0.3f, 0, 0)));
        gameObjects.push_back(new Log(Vector3(7, 0, -2), Vector3(0.4f, 0, 0)));

        // Custom objects with saved pointers
        gameObjects.push_back(directionalLight);
        directionalLight->setEnabled(false);

        gameObjects.push_back(bus);
        gameObjects.push_back(log);
        gameObjects.push_back(player);
        gameObjects.push_back(cmin);
        gameObjects.push_back(cmax);
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
            // case 'c': cameraPerspectiveFollow.printCoordinates(); break;
            // Quality settings
            case 'm': glEnable(GL_MULTISAMPLE); break;
            case 'M': glDisable(GL_MULTISAMPLE); break;

            // Player movement
            case 'p': movePlayer(1, 0); break;
            case 'o': movePlayer(-1, 0); break;
            case 'q': movePlayer(0, -1); break;
            case 'a': movePlayer(0, 1); break;

            // Night mode toggle
            case 'n':
                directionalLight->setEnabled(!directionalLight->isEnabled()); break;
            // Stop/Continue game
            case 's': isPlaying = !isPlaying; break;

            // CameraType
            case '1': selectCamera(CAMERA_PERSPECTIVE_FOLLOW); break;
            case '2': selectCamera(CAMERA_PERSPECTIVE_FIXED); break;
            case '3': selectCamera(CAMERA_ORTHO); break;
        }
    }

    void movePlayer(float x, float z) {
        GLint curTime = glutGet(GLUT_ELAPSED_TIME);

        if (curTime - lastMoveTime < moveTimeout)
            return;
        else if(!isPlaying)
            return;

        Vector3 moveVec = Vector3(x, 0, z);
        Vector3 tempPos = player->position + moveVec;
        if(tempPos.z < topVerticalLimitPlayerPos || tempPos.z > bottomVerticalLimitPlayerPos ||
           tempPos.x > rightHorizontalLimitPlayerPos || tempPos.x < leftHorizontalLimitPlayerPos)
            return;

        player->jump(moveVec, 0.5f);
        lastMoveTime = curTime;
    }

    GLuint setupShaders()
    {
        // Shader for models
        shader.init();
        shader.loadShader(VSShaderLib::VERTEX_SHADER, VERTEX_SHADER_PATH);
        shader.loadShader(VSShaderLib::FRAGMENT_SHADER, FRAGMENT_SHADER_PATH);

        // set semantics for the shader variables
        glBindFragDataLocation(shader.getProgramIndex(), 0,"colorOut");
        glBindAttribLocation(shader.getProgramIndex(), VERTEX_COORD_ATTRIB, "position");
        glBindAttribLocation(shader.getProgramIndex(), NORMAL_ATTRIB, "normal");

        glLinkProgram(shader.getProgramIndex());

        pvm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_pvm");
        vm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_viewModel");
        normal_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_normal");
        lPos_uniformId = glGetUniformLocation(shader.getProgramIndex(), "l_pos");

        printf("InfoLog for Per Fragment Phong Lightning Shader\n%s\n\n", shader.getAllInfoLogs().c_str());
        return(shader.isProgramLinked());
    }

    void initScene()
    {
        // Initialize all of the GameObjects
        for (GameObject* go : gameObjects) {
            go->init();
        }

        // some GL settings
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_MULTISAMPLE);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    }

    void renderScene() {
        GLint currentTime = glutGet(GLUT_ELAPSED_TIME);
        deltaTime = prevTime - currentTime;
        prevTime = currentTime;

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

        if(isPlaying) {
            // Draw the testing coordinates
            //cmax->position = player->position + player->boundingBox.vecMax;
            //cmin->position = player->position + player->boundingBox.vecMin;
        }

        // use our shader
        glUseProgram(shader.getProgramIndex());

        if (!shader.isProgramValid()) {
            printf("Program Not Valid!\n");
            exit(1);
        }

        bool roadDeath = false;
        bool hitRiver = false;
        bool hitLog = false;

        for (GameObject *go : gameObjects) {
            if (go->isEnabled()) {
                // Update the physics
                if (isPlaying) go->update(deltaTime);

                // Render the objects
                go->render();

                // Check collisions with player
                if (!(go->position == player->position)) {
                    if (player->collideWith(go)) {
                        if (go->getType() == BUS) {
                            roadDeath = true;
                        }
                    }
                    else if ((player->playerState == GROUNDED) && (player->collideWithBottom(go))) {
                        // cout << "Bottom collision with " << go->getType() << endl;
                        if (go->getType() == LOG) {
                            hitLog = true;
                            player->playerState = ONLOG;
                            player->speed = go->getSpeed();
                        } else if (go->getType() == RIVER) {
                            hitRiver = true;
                        }
                    }
                }
            }
        }

        bool deathInRiver = hitRiver && (!hitLog) && (player->playerState != ONLOG);

        if (deathInRiver) {
            cout << "Death in river!" << endl;
        }
        if (roadDeath) {
            cout << "Death on the road!" << endl;
        }

        glutSwapBuffers();
    }

private:
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
};

#endif //AVT7_GAMEMANAGER_H
