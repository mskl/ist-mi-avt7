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
#include "CustomMaterial.h"

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
#include "objects/PointLight.h"


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

    enum CameraType {
        CAMERA_PERSPECTIVE_FOLLOW,
        CAMERA_PERSPECTIVE_FIXED,
        CAMERA_ORTHO
    };

    CameraType selectedCamera = CAMERA_PERSPECTIVE_FOLLOW;
    CameraPerspectiveMoving cameraPerspectiveMoving = CameraPerspectiveMoving(20, 0, 40);
    CameraPerspective cameraPerspectiveFixed = CameraPerspective(20, 0, 90);
    CameraOrthogonal cameraOrthogonal = CameraOrthogonal(-15, 15, -15, 15,
            Vector3(0, 20, 0));

    River river = River();
    Ground ground = Ground();
    Player player = Player();
    Road road = Road();
    PointLight pointLight = PointLight();

public:
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
        // printf("Key value (%d)\n", key);

        // TODO: This should be extracted to the player update function
        switch(key) {
            // Escape exits the game
            case 27: glutLeaveMainLoop(); break;
            // case 'c': cameraPerspectiveFollow.printCoordinates(); break;
            // Quality settings
            case 'm': glEnable(GL_MULTISAMPLE); break;
            case 'n': glDisable(GL_MULTISAMPLE); break;

            // Player movement
            case 'p': movePlayerHorizontally(1); break;
            case 'o': movePlayerHorizontally(-1); break;
            case 'q': movePlayerVertically(-1); break;
            case 'a': movePlayerVertically(1); break;

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

    void movePlayerVertically(float dir){
        if(!isPlaying)
            return;

        float tempPos = player.position.z + dir;

        if(tempPos < topVerticalLimitPlayerPos || tempPos > bottomVerticalLimitPlayerPos)
            return;

        player.position.z = tempPos;
    }

    void movePlayerHorizontally(float dir){
        if(!isPlaying)
            return;

        float tempPos = player.position.x + dir;

        if(tempPos > rightHorizontalLimitPlayerPos || tempPos < leftHorizontalLimitPlayerPos)
            return;

        player.position.x = tempPos;
    }

    GLuint setupShaders()
    {
        // Shader for models
        shader.init();
        shader.loadShader(VSShaderLib::VERTEX_SHADER, "shaders/pointlight.vert");
        shader.loadShader(VSShaderLib::FRAGMENT_SHADER, "shaders/pointlight.frag");

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

    void renderScene(void) {
        GLint loc;

        FrameCount++;
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        loadIdentity(VIEW);
        loadIdentity(MODEL);

        if (selectedCamera == CAMERA_PERSPECTIVE_FOLLOW) {
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

        river.render();
        ground.render();
        player.render();
        road.render();
        pointLight.render();

        glutSwapBuffers();
    }

    void initScene()
    {
        river.init();
        ground.init();
        player.init();
        road.init();
        pointLight.init();

        // some GL settings
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_MULTISAMPLE);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    }
};

#endif //AVT7_GAMEMANAGER_H
