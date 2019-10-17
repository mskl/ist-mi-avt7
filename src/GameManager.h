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
#include "basic_geometry.h"
#include "libs/vsShaderLib.h"
#include "Camera.h"
#include "CameraPerspective.h"
#include "CameraPerspectiveMoving.h"
#include "CameraOrthogonal.h"

#define CAPTION "AVT Per Fragment Phong Lightning Demo"
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
extern float mNormal3x3[9];

int objId = 0;

struct MyMesh mesh[10];

class GameManager {
public:
    int WindowHandle = 0;
    int WinX = 1280, WinY = 960;

    unsigned int FrameCount = 0;
    VSShaderLib shader;

    GLint pvm_uniformId;
    GLint vm_uniformId;
    GLint normal_uniformId;
    GLint lPos_uniformId;

    float lightPos[4] = {4.0f, 6.0f, 2.0f, 1.0f};
    float playerPos[3] = {0.0f, 1.0f, 0.0f};

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
        printf("Key value (%d)\n", key);

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

        float tempPos = playerPos[2] + dir;

        if(tempPos < topVerticalLimitPlayerPos || tempPos > bottomVerticalLimitPlayerPos)
            return;

        playerPos[2] = tempPos;
    }

    void movePlayerHorizontally(float dir){
        if(!isPlaying)
            return;

        float tempPos = playerPos[0] + dir;

        if(tempPos > rightHorizontalLimitPlayerPos || tempPos < leftHorizontalLimitPlayerPos)
            return;

        playerPos[0] = tempPos;
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

        // Setup the pointLight
        renderObjectLight();
        renderObjectGround();
        renderObjectRoad();
        renderObjectRiver();
        renderObjectPlayer();

        glutSwapBuffers();
    }

    void initScene()
    {
        initObjectGround();
        initObjectPlayer();
        initObjectRoad();
        initObjectRiver();

        // some GL settings
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_MULTISAMPLE);
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
    }

private:
    void renderObjectLight() {
        float res[4];
        multMatrixPoint(VIEW, lightPos, res);
        glUniform4fv(lPos_uniformId, 1, res);
    }

    void renderObjectPlayer() {
        objId = 5; // Player
        GLint loc_amb = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
        glUniform4fv(loc_amb, 1, mesh[objId].mat.ambient);
        GLint loc_dif = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
        glUniform4fv(loc_dif, 1, mesh[objId].mat.diffuse);
        GLint loc_spc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
        glUniform4fv(loc_spc, 1, mesh[objId].mat.specular);
        GLint loc_shi = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
        glUniform1f(loc_shi, mesh[objId].mat.shininess);

        pushMatrix(MODEL);
        translate(MODEL, 0.5, 0.5, 0.5);
        translate(MODEL, playerPos[0], playerPos[1], playerPos[2]);
        computeDerivedMatrix(PROJ_VIEW_MODEL);
        glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
        glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
        computeNormalMatrix3x3();
        glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
        glBindVertexArray(mesh[objId].vao);
        glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        popMatrix(MODEL);
    }

    void renderObjectRiver() {
        objId = 3;
        GLint loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
        glUniform4fv(loc, 1, mesh[objId].mat.ambient);
        loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
        glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
        loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
        glUniform4fv(loc, 1, mesh[objId].mat.specular);
        loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
        glUniform1f(loc, mesh[objId].mat.shininess);
        pushMatrix(MODEL);

        translate(MODEL, -13 / 2, 0, -5);
        scale(MODEL, 13, 1, 5);

        computeDerivedMatrix(PROJ_VIEW_MODEL);
        glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
        glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
        computeNormalMatrix3x3();
        glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
        glBindVertexArray(mesh[objId].vao);
        glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        popMatrix(MODEL);
    }

    void renderObjectRoad() {
        objId = 1;
        GLint loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
        glUniform4fv(loc, 1, mesh[objId].mat.ambient);
        loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
        glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
        loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
        glUniform4fv(loc, 1, mesh[objId].mat.specular);
        loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
        glUniform1f(loc, mesh[objId].mat.shininess);
        pushMatrix(MODEL);

        translate(MODEL, -13 / 2, 0, 1);
        scale(MODEL, 13, 1, 5);

        computeDerivedMatrix(PROJ_VIEW_MODEL);
        glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
        glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
        computeNormalMatrix3x3();
        glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
        glBindVertexArray(mesh[objId].vao);
        glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        popMatrix(MODEL);
    }

    void renderObjectGround() {
        objId = 0;
        GLint loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
        glUniform4fv(loc, 1, mesh[objId].mat.ambient);
        loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
        glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
        loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
        glUniform4fv(loc, 1, mesh[objId].mat.specular);
        loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
        glUniform1f(loc, mesh[objId].mat.shininess);

        pushMatrix(MODEL);
        translate(MODEL, -13 / 2, 0, 6);
        scale(MODEL, 13, 1, 1);
        computeDerivedMatrix(PROJ_VIEW_MODEL);
        glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
        glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
        computeNormalMatrix3x3();
        glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
        glBindVertexArray(mesh[objId].vao);
        glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        popMatrix(MODEL);

        objId = 2;
        loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
        glUniform4fv(loc, 1, mesh[objId].mat.ambient);
        loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
        glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
        loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
        glUniform4fv(loc, 1, mesh[objId].mat.specular);
        loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
        glUniform1f(loc, mesh[objId].mat.shininess);
        pushMatrix(MODEL);

        translate(MODEL, -13 / 2, 0, 0);
        scale(MODEL, 13, 1, 1);

        computeDerivedMatrix(PROJ_VIEW_MODEL);
        glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
        glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
        computeNormalMatrix3x3();
        glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
        glBindVertexArray(mesh[objId].vao);
        glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        popMatrix(MODEL);

        objId = 4;
        loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
        glUniform4fv(loc, 1, mesh[objId].mat.ambient);
        loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
        glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
        loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
        glUniform4fv(loc, 1, mesh[objId].mat.specular);
        loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
        glUniform1f(loc, mesh[objId].mat.shininess);

        pushMatrix(MODEL);
        translate(MODEL, -13 / 2, 0, -6);
        scale(MODEL, 13, 1, 1);
        computeDerivedMatrix(PROJ_VIEW_MODEL);
        glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
        glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
        computeNormalMatrix3x3();
        glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
        glBindVertexArray(mesh[objId].vao);
        glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        popMatrix(MODEL);
    }

    void initObjectGround()
    {
        objId=0; // Bottom ground
        memcpy(mesh[objId].mat.ambient, customMaterial.amb, 4 * sizeof(float));
        memcpy(mesh[objId].mat.diffuse, customMaterial.diff_green, 4 * sizeof(float));
        memcpy(mesh[objId].mat.specular, customMaterial.spec, 4 * sizeof(float));
        memcpy(mesh[objId].mat.emissive, customMaterial.emissive, 4 * sizeof(float));
        mesh[objId].mat.shininess = customMaterial.shininess;
        mesh[objId].mat.texCount = customMaterial.texcount;
        createCube();

        objId=2; // Land2 - middle
        memcpy(mesh[objId].mat.ambient, customMaterial.amb, 4 * sizeof(float));
        memcpy(mesh[objId].mat.diffuse, customMaterial.diff_green, 4 * sizeof(float));
        memcpy(mesh[objId].mat.specular, customMaterial.spec, 4 * sizeof(float));
        memcpy(mesh[objId].mat.emissive, customMaterial.emissive, 4 * sizeof(float));
        mesh[objId].mat.shininess = customMaterial.shininess;
        mesh[objId].mat.texCount = customMaterial.texcount;
        createCube();

        objId=4; // Land3 - top
        memcpy(mesh[objId].mat.ambient, customMaterial.amb, 4 * sizeof(float));
        memcpy(mesh[objId].mat.diffuse, customMaterial.diff_green, 4 * sizeof(float));
        memcpy(mesh[objId].mat.specular, customMaterial.spec, 4 * sizeof(float));
        memcpy(mesh[objId].mat.emissive, customMaterial.emissive, 4 * sizeof(float));
        mesh[objId].mat.shininess = customMaterial.shininess;
        mesh[objId].mat.texCount = customMaterial.texcount;
        createCube();
    }

    void initObjectRoad()
    {
        objId=1; // Road
        memcpy(mesh[objId].mat.ambient, customMaterial.amb, 4 * sizeof(float));
        memcpy(mesh[objId].mat.diffuse, customMaterial.diff_black, 4 * sizeof(float));
        memcpy(mesh[objId].mat.specular, customMaterial.spec, 4 * sizeof(float));
        memcpy(mesh[objId].mat.emissive, customMaterial.emissive, 4 * sizeof(float));
        mesh[objId].mat.shininess = customMaterial.shininess;
        mesh[objId].mat.texCount = customMaterial.texcount;
        createCube();
    }

    void initObjectRiver()
    {
        objId=3; // River
        memcpy(mesh[objId].mat.ambient, customMaterial.amb, 4 * sizeof(float));
        memcpy(mesh[objId].mat.diffuse, customMaterial.diff_blue, 4 * sizeof(float));
        memcpy(mesh[objId].mat.specular, customMaterial.spec, 4 * sizeof(float));
        memcpy(mesh[objId].mat.emissive, customMaterial.emissive, 4 * sizeof(float));
        mesh[objId].mat.shininess = customMaterial.shininess;
        mesh[objId].mat.texCount = customMaterial.texcount;
        createCube();
    }

    void initObjectPlayer()
    {
        objId=5; // Player
        memcpy(mesh[objId].mat.ambient, customMaterial.amb, 4 * sizeof(float));
        memcpy(mesh[objId].mat.diffuse, customMaterial.diff, 4 * sizeof(float));
        memcpy(mesh[objId].mat.specular, customMaterial.spec, 4 * sizeof(float));
        memcpy(mesh[objId].mat.emissive, customMaterial.emissive, 4 * sizeof(float));
        mesh[objId].mat.shininess = customMaterial.shininess;
        mesh[objId].mat.texCount = customMaterial.texcount;
        createSphere(0.5f, 20);
    }
};

#endif //AVT7_GAMEMANAGER_H
