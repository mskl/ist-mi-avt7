//
// Created by skalimat on 17.10.19.
//

#ifndef AVT7_RIVER_H
#define AVT7_RIVER_H

#include "../GameObject.h"
#include <stdlib.h>
#include "GL/glew.h"
#include "../libs/vsShaderLib.h"
#include "../libs/AVTmathLib.h"
#include "../CustomMaterial.h"
#include "../libs/basic_geometry.h"
#include <string.h>

class River: public GameObject {
public:

    River(Vector3 pos, int id): GameObject(pos, id) {

    }

    void init() {
        objId=id; // River
        memcpy(mesh[objId].mat.ambient, customMaterial.amb, 4 * sizeof(float));
        memcpy(mesh[objId].mat.diffuse, customMaterial.diff_blue, 4 * sizeof(float));
        memcpy(mesh[objId].mat.specular, customMaterial.spec, 4 * sizeof(float));
        memcpy(mesh[objId].mat.emissive, customMaterial.emissive, 4 * sizeof(float));
        mesh[objId].mat.shininess = customMaterial.shininess;
        mesh[objId].mat.texCount = customMaterial.texcount;
        createCube(id);
    }

    void render() {
        objId=id;
        GLint loc = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
        glUniform4fv(loc, 1, mesh[objId].mat.ambient);
        loc = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
        glUniform4fv(loc, 1, mesh[objId].mat.diffuse);
        loc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
        glUniform4fv(loc, 1, mesh[objId].mat.specular);
        loc = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
        glUniform1f(loc, mesh[objId].mat.shininess);
        pushMatrix(MODEL);

        translate(MODEL, position.x, position.y, position.z);
        translate(MODEL, -13 / 2, 0, 0);
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
};

/**/
#endif //AVT7_RIVER_H
