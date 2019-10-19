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


const char* VERTEX_SHADER_PATH = "shaders/pointlight.vert";
const char* FRAGMENT_SHADER_PATH = "shaders/pointlight.frag";

extern int deltaTime;
GLint pvm_uniformId;
GLint vm_uniformId;
GLint normal_uniformId;
GLint lPos_uniformId;


#define CAPTION "AVT Per Fragment Phong Lightning Demo"

class GameManager {
public:
    int WindowHandle = 0;
    int WinX = 1280, WinY = 960;

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

    CameraType selectedCamera = CAMERA_PERSPECTIVE_FOLLOW;
    CameraPerspectiveMoving cameraPerspectiveMoving
        = CameraPerspectiveMoving(20, 0, 40, Vector3(0, 20, 0));
    CameraPerspective cameraPerspectiveFixed
        = CameraPerspective(20, 0, 90, Vector3(0, 20, 0));
    CameraOrthogonal cameraOrthogonal
        = CameraOrthogonal(-15, 15, -15, 15,Vector3(0, 20, 0));

    Player* player = new Player(Vector3(0, 1, 0));
    Light* directionalLight = new Light(Vector3(0.0f, -0.1f, 0.0f), 0);
    Bus* bus = new Bus(Vector3(6, 1, 0), Vector3(1, 0, 0));

public:
    GameManager() {
        gameObjects.push_back(new River(Vector3(0, 0, -5)));
        gameObjects.push_back(new Road(Vector3(0, 0, 1)));
        gameObjects.push_back(new Ground(Vector3(0, 0, 0)));
        gameObjects.push_back(new Light(Vector3(4.0f, 6.0f, 2.0f), 1));
        gameObjects.push_back(new Coordinates(Vector3(0, 0, 0)));

        gameObjects.push_back(new Coordinates(Vector3(6.5, 0, 6.5)));
        gameObjects.push_back(new Coordinates(Vector3(6.5, 0, -6.5)));
        gameObjects.push_back(new Coordinates(Vector3(-6.5, 0, 6.5)));
        gameObjects.push_back(new Coordinates(Vector3(-6.5, 0, -6.5)));

        // Custom objects with saved pointers
        gameObjects.push_back(bus);
        gameObjects.push_back(directionalLight);
        directionalLight->setEnabled(false);
        gameObjects.push_back(player);
    }

    void changeSize(int w, int h)
    {
        float ratio;
        // Prevent a divide by zero, when window is too short
        if (h == 0) h = 1;

        // set the viewport to be the entire window
        glViewport(0, 0, w, h);
        loadIdentity(PROJECTION);

        if (selectedCamera == CAMERA_PERSPECTIVE_FOLLOW) {
            cameraPerspectiveMoving.project(w, h);
        } else if (selectedCamera == CAMERA_PERSPECTIVE_FIXED) {
           cameraPerspectiveFixed.project(w, h);
        } else if (selectedCamera == CAMERA_ORTHO) {
            cameraOrthogonal.project(w, h);
        }
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
            case '1':
                selectedCamera = CAMERA_PERSPECTIVE_FOLLOW; break;
            case '2':
                selectedCamera = CAMERA_PERSPECTIVE_FIXED; break;
            case '3':
                selectedCamera = CAMERA_ORTHO; break;
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

        player->jump(tempPos, 0.5f);
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
        GLint loc;

        FrameCount++;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        loadIdentity(VIEW);
        loadIdentity(MODEL);

        if (selectedCamera == CAMERA_PERSPECTIVE_FOLLOW) {
            cameraPerspectiveMoving.pos = player->position;
            cameraPerspectiveMoving.view();
        } else if (selectedCamera == CAMERA_PERSPECTIVE_FIXED) {
            cameraPerspectiveFixed.view();
        } else if (selectedCamera == CAMERA_ORTHO) {
            cameraOrthogonal.view();
        }

        // use our shader
        glUseProgram(shader.getProgramIndex());

        if (!shader.isProgramValid()) {
            printf("Program Not Valid!\n");
            exit(1);
        }

        // Render all of the GameObjects
        for (GameObject *go : gameObjects) {
            if (go->isEnabled()) {
                go->update(deltaTime);
                go->render();

                // Check collisions with player
                if (!(go->position == player->position)) {
                    bool didCollide = player->collideWith(go);
                    if(didCollide) {
                        cout << "COLLISION!" << endl;
                    }
                }
            }
        }

        glutSwapBuffers();
    }
};

#endif //AVT7_GAMEMANAGER_H
