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
#include "basic_geometry.h"
#include "libs/vsShaderLib.h"

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

    float camX, camY, camZ;
    int startX, startY, tracking = 0;
    float alpha = 0.0f, beta = 40.0f, r = 20.0f;
    float lightPos[4] = {4.0f, 6.0f, 2.0f, 1.0f};
    float playerPos[3] = {0.0f, 1.0f, 0.0f};
    int cameraType = 1;

public:
    GameManager() {
        // Setup a GameManager
    }

    ~GameManager() = default;

    void changeSize(int w, int h)
    {
        float ratio;
        // Prevent a divide by zero, when window is too short
        if (h == 0) {
            h = 1;
        }

        // set the viewport to be the entire window
        glViewport(0, 0, w, h);

        // set the projection matrix
        ratio = (1.0f * w) / h;
        loadIdentity(PROJECTION);

        if (cameraType == 1) {
            perspective(53.13f, ratio, 0.1f, 1000.0f);
        } else if (cameraType == 2) {
            ortho(-13, 13, -13, 13, -100, 100);
        }
    }

    void processKeys(unsigned char key, int xx, int yy)
    {
        printf("Key value (%d)\n", key);
        switch(key) {
            // Escape exits the game
            case 27: glutLeaveMainLoop(); break;
            case 'c': printf("Camera Spherical Coordinates (%f, %f, %f)\n", alpha, beta, r); break;
            // Quality settings
            case 'm': glEnable(GL_MULTISAMPLE); break;
            case 'n': glDisable(GL_MULTISAMPLE); break;
            // Player movement
            case 'p': playerPos[0] += 1; break;
            case 'o': playerPos[0] -= 1; break;
            case 'q': playerPos[2] -= 1; break;
            case 'a': playerPos[2] += 1; break;
            // CameraType
            case '1': cameraType = 1; break;
            case '2': cameraType = 2; break;
            case '3': cameraType = 3; break;
        }
    }

    void processMouseButtons(int button, int state, int xx, int yy)
    {
        // start tracking the mouse
        if (state == GLUT_DOWN)  {
            startX = xx;
            startY = yy;
            if (button == GLUT_LEFT_BUTTON)
                tracking = 1;
            else if (button == GLUT_RIGHT_BUTTON)
                tracking = 2;
        }

        // stop tracking the mouse
        else if (state == GLUT_UP) {
            if (tracking == 1) {
                alpha -= (xx - startX);
                beta += (yy - startY);
            }
            else if (tracking == 2) {
                r += (yy - startY) * 0.01f;
                if (r < 0.1f)
                    r = 0.1f;
            }
            tracking = 0;
        }
    }

    void processMouseMotion(int xx, int yy)
    {
        int deltaX, deltaY;
        float alphaAux, betaAux;
        float rAux;

        deltaX =  - xx + startX;
        deltaY =    yy - startY;

        // left mouse button: move camera
        if (tracking == 1) {
            alphaAux = alpha + deltaX;
            betaAux = beta + deltaY;

            if (betaAux > 85.0f)
                betaAux = 85.0f;
            else if (betaAux < -85.0f)
                betaAux = -85.0f;
            rAux = r;
        }
        // right mouse button: zoom
        else if (tracking == 2) {
            alphaAux = alpha;
            betaAux = beta;
            rAux = r + (deltaY * 0.01f);
            if (rAux < 0.1f)
                rAux = 0.1f;
        }

        camX = rAux * sin(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
        camZ = rAux * cos(alphaAux * 3.14f / 180.0f) * cos(betaAux * 3.14f / 180.0f);
        camY = rAux *   						          sin(betaAux * 3.14f / 180.0f);
    }

    void mouseWheel(int wheel, int direction, int x, int y)
    {
        r += direction * 0.1f;
        if (r < 0.1f)
            r = 0.1f;

        camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
        camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
        camY = r *   						        sin(beta * 3.14f / 180.0f);
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

        // set the camera using a function similar to gluLookAt
        lookAt(camX, camY, camZ, 0, 0, 0, 0, 1, 0);

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
        initObjectCamera();
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
        GLint loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
        glUniform4fv(loc, 1, mesh[objId].mat.ambient);
        loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
        glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
        loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
        glUniform4fv(loc, 1, mesh[objId].mat.specular);
        loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
        glUniform1f(loc, mesh[objId].mat.shininess);

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
        memcpy(mesh[objId].mat.ambient, material.amb,4*sizeof(float));
        memcpy(mesh[objId].mat.diffuse, material.diff_green,4*sizeof(float));
        memcpy(mesh[objId].mat.specular, material.spec,4*sizeof(float));
        memcpy(mesh[objId].mat.emissive, material.emissive,4*sizeof(float));
        mesh[objId].mat.shininess = material.shininess;
        mesh[objId].mat.texCount = material.texcount;
        createCube();

        objId=2; // Land2 - middle
        memcpy(mesh[objId].mat.ambient, material.amb,4*sizeof(float));
        memcpy(mesh[objId].mat.diffuse, material.diff_green,4*sizeof(float));
        memcpy(mesh[objId].mat.specular, material.spec,4*sizeof(float));
        memcpy(mesh[objId].mat.emissive, material.emissive,4*sizeof(float));
        mesh[objId].mat.shininess = material.shininess;
        mesh[objId].mat.texCount = material.texcount;
        createCube();

        objId=4; // Land3 - top
        memcpy(mesh[objId].mat.ambient, material.amb,4*sizeof(float));
        memcpy(mesh[objId].mat.diffuse, material.diff_green,4*sizeof(float));
        memcpy(mesh[objId].mat.specular, material.spec,4*sizeof(float));
        memcpy(mesh[objId].mat.emissive, material.emissive,4*sizeof(float));
        mesh[objId].mat.shininess = material.shininess;
        mesh[objId].mat.texCount = material.texcount;
        createCube();
    }

    void initObjectRoad()
    {
        objId=1; // Road
        memcpy(mesh[objId].mat.ambient, material.amb,4*sizeof(float));
        memcpy(mesh[objId].mat.diffuse, material.diff_black,4*sizeof(float));
        memcpy(mesh[objId].mat.specular, material.spec,4*sizeof(float));
        memcpy(mesh[objId].mat.emissive, material.emissive,4*sizeof(float));
        mesh[objId].mat.shininess = material.shininess;
        mesh[objId].mat.texCount = material.texcount;
        createCube();
    }

    void initObjectRiver()
    {
        objId=3; // River
        memcpy(mesh[objId].mat.ambient, material.amb,4*sizeof(float));
        memcpy(mesh[objId].mat.diffuse, material.diff_blue,4*sizeof(float));
        memcpy(mesh[objId].mat.specular, material.spec,4*sizeof(float));
        memcpy(mesh[objId].mat.emissive, material.emissive,4*sizeof(float));
        mesh[objId].mat.shininess = material.shininess;
        mesh[objId].mat.texCount = material.texcount;
        createCube();
    }

    void initObjectPlayer()
    {
        objId=5; // Player
        memcpy(mesh[objId].mat.ambient, material.amb,4*sizeof(float));
        memcpy(mesh[objId].mat.diffuse, material.diff,4*sizeof(float));
        memcpy(mesh[objId].mat.specular, material.spec,4*sizeof(float));
        memcpy(mesh[objId].mat.emissive, material.emissive,4*sizeof(float));
        mesh[objId].mat.shininess = material.shininess;
        mesh[objId].mat.texCount = material.texcount;
        createSphere(0.5f, 20);
    }

    void initObjectCamera()
    {
        // set the camera position based on its spherical coordinates
        camX = r * sin(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
        camZ = r * cos(alpha * 3.14f / 180.0f) * cos(beta * 3.14f / 180.0f);
        camY = r *   						     sin(beta * 3.14f / 180.0f);
    }

    struct Material{
        float amb[4] = {0.2f, 0.15f, 0.1f, 1.0f};
        float diff[4] = {0.8f, 0.6f, 0.4f, 1.0f};

        float diff_black[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        float diff_blue[4] = {0.0f, 0.0f, 1.0f, 1.0f};
        float diff_green[4] = {0.0f, 1.0f, 0.0f, 1.0f};

        float spec[4] = {0.8f, 0.8f, 0.8f, 1.0f};
        float emissive[4] = {0.0f, 0.0f, 0.0f, 1.0f};
        float shininess= 10.0f;
        int texcount = 0;

    }; Material material;
};

#endif //AVT7_GAMEMANAGER_H
