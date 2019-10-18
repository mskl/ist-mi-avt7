//
// Created by skalimat on 11.10.19.
//

#ifndef AVT7_GAMEOBJECT_H
#define AVT7_GAMEOBJECT_H

#include "libs/basic_geometry.h"
#include <tuple>
#include "Vector3.h"
#include "ShaderIndices.h"


extern struct MyMesh mesh[];
extern VSShaderLib shader;
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
extern float mNormal3x3[9];

extern GLint pvm_uniformId;
extern GLint vm_uniformId;
extern GLint normal_uniformId;
extern GLint lPos_uniformId;


class GameObject {
public:
    std::vector<int> ids = {};
    Vector3 position;

    // Static counter of the highest assigned id
    static int idCount;

    GameObject(Vector3 pos) {
        this->position = pos;
    }

    virtual void init() { }
    virtual void render() { }

    void update() { }

    static void renderMaterials(GLint mid) {
        ShaderIndices si = getPointers(mid);
        glUniform4fv(si.loc_amb, 1, mesh[mid].mat.ambient);
        glUniform4fv(si.loc_dif, 1, mesh[mid].mat.diffuse);
        glUniform4fv(si.loc_spc, 1, mesh[mid].mat.specular);
        glUniform1f(si.loc_shi, mesh[mid].mat.shininess);
    }

    static void setMesh(GLint mid, float amb[4], float dif[4], float spec[4], float emmit[4],
                 float shinnines, float texcount) {
        memcpy(mesh[mid].mat.ambient, amb, 4 * sizeof(float));
        memcpy(mesh[mid].mat.diffuse, dif, 4 * sizeof(float));
        memcpy(mesh[mid].mat.specular, spec, 4 * sizeof(float));
        memcpy(mesh[mid].mat.emissive, emmit, 4 * sizeof(float));
        mesh[mid].mat.shininess = shinnines;
        mesh[mid].mat.texCount = texcount;
    }

    static ShaderIndices getPointers(GLint mid) {
        GLint loc_amb = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
        glUniform4fv(loc_amb, 1, mesh[mid].mat.ambient);
        GLint loc_dif = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
        glUniform4fv(loc_dif, 1, mesh[mid].mat.diffuse);
        GLint loc_spc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
        glUniform4fv(loc_spc, 1, mesh[mid].mat.specular);
        GLint loc_shi = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
        glUniform1f(loc_shi, mesh[mid].mat.shininess);

        return {loc_amb, loc_dif, loc_spc, loc_shi};
    }

    static void buildVAO(GLint mid) {
        computeDerivedMatrix(PROJ_VIEW_MODEL);
        glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
        glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
        computeNormalMatrix3x3();
        glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
        glBindVertexArray(mesh[mid].vao);
        glDrawElements(mesh[mid].type, mesh[mid].numIndexes, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
};

// Track how many IDs were used
int GameObject::idCount = 0;

#endif //AVT7_GAMEOBJECT_H
