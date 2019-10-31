//
// Created by skalimat on 29.10.19.
//

#ifndef AVT7_SCENESTENCIL_H
#define AVT7_SCENESTENCIL_H

#include "../GameObject.h"

class SceneStencil: public GameObject {
public:
    SceneStencil(): GameObject(Vector3()) {

    }

    void init() override {
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_ground);
        createCube(ids.back());
    }

    void render() override {
        pushMatrix(MODEL);
        renderMaterials(ids[0]);
        translate(MODEL, -6, -3, -6);
        scale(MODEL,     13, 10, 13);

        computeDerivedMatrix(PROJ_VIEW_MODEL);
        glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
        glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
        computeNormalMatrix3x3();
        glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

        if (USE_STENCIL) {
            glClear(GL_STENCIL_BUFFER_BIT);
            // Always fails
            glStencilFunc(GL_NEVER, 0x1, 0x1);

            // actions to change stencil buffer
            // GL_REPLACE = set the value in stencil buffer to ref in glStencilFunc()
            glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);
        }

        glBindVertexArray(mesh[ids[0]].vao);
        glDrawElements(mesh[ids[0]].type, mesh[ids[0]].numIndexes, GL_UNSIGNED_INT, 0);
        popMatrix(MODEL);
    }
};

#endif //AVT7_SCENESTENCIL_H
