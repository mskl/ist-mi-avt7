//
// Created by skalimat on 17.10.19.
//

#ifndef AVT7_PLAYER_H
#define AVT7_PLAYER_H

#include "../GameObject.h"

class Player: public GameObject {
public:

    Player() {
        position = Vector3(0.0f, 1.0f, 0.0f);
    }

    void init() {
        objId=5; // Player
        memcpy(mesh[objId].mat.ambient, customMaterial.amb, 4 * sizeof(float));
        memcpy(mesh[objId].mat.diffuse, customMaterial.diff, 4 * sizeof(float));
        memcpy(mesh[objId].mat.specular, customMaterial.spec, 4 * sizeof(float));
        memcpy(mesh[objId].mat.emissive, customMaterial.emissive, 4 * sizeof(float));
        mesh[objId].mat.shininess = customMaterial.shininess;
        mesh[objId].mat.texCount = customMaterial.texcount;
        createSphere(0.5f, 20);
    }

    void render() {
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
        translate(MODEL, position.x, position.y, position.z);
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
};

#endif //AVT7_PLAYER_H
