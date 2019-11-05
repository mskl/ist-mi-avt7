//
// Created by skalimat on 11.10.19.
//

#ifndef AVT7_GAMEOBJECT_H
#define AVT7_GAMEOBJECT_H

#include "libs/basic_geometry.h"
#include "Vector3.h"
#include "ShaderIndices.h"
#include "BoundingBox.h"
#include "materials/Materials.h"
#include "GameManager.h"
#include "libs/vsShaderLib.h"

#define USE_STENCIL 0

// Macro to print filename when using std::cout
#define mycout std::cout <<  __FILE__  << "(" << __LINE__ << ") "
#define cout mycout

extern struct MyMesh mesh[];
extern VSShaderLib shader;
extern float mMatrix[COUNT_MATRICES][16];
extern float mCompMatrix[COUNT_COMPUTED_MATRICES][16];
extern float mNormal3x3[9];

extern GLint pvm_uniformId;
extern GLint vm_uniformId;
extern GLint normal_uniformId;
extern GLint l_pos_id[8];

enum GameObjectType {BOUNDS, TARGET, LOG, TURTLE, RIVER, GROUND, BUS,CAR, UNKNOWN, PLAYER, DEADLYBOUNDS};

class GameObject {
protected:
    bool enabled = true;
    bool animationEnabled = true;
    std::vector<int> ids = {};

    // Static counter of the highest assigned id
    static int idCount;

    static void renderMaterials(GLint mid) {
        ShaderIndices si = getPointers(mid);
        glUniform4fv(si.loc_amb, 1, mesh[mid].mat.ambient);
        glUniform4fv(si.loc_dif, 1, mesh[mid].mat.diffuse);
        glUniform4fv(si.loc_spc, 1, mesh[mid].mat.specular);
        glUniform1f(si.loc_shi, mesh[mid].mat.shininess);
        glUniform1i(si.loc_txcnt, mesh[mid].mat.texcount);
    }

    static void renderTexture(GLint mid, int mode=0){
        glUniform1i(mid, mode);
    }

    static void setMaterial(GLint mid, AMaterial mat) {
        memcpy(mesh[mid].mat.ambient, mat.amb, 4 * sizeof(float));
        memcpy(mesh[mid].mat.diffuse, mat.diff, 4 * sizeof(float));
        memcpy(mesh[mid].mat.specular, mat.spec, 4 * sizeof(float));
        memcpy(mesh[mid].mat.emissive, mat.emissive, 4 * sizeof(float));
        mesh[mid].mat.shininess = mat.shininess;
        mesh[mid].mat.texcount = mat.texcount;
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
        GLint loc_txcnt = glGetUniformLocation(shader.getProgramIndex(), "mat.texcount");
        glUniform1f(loc_txcnt, mesh[mid].mat.texcount);
        return {loc_amb, loc_dif, loc_spc, loc_shi, loc_txcnt};
    }

    virtual void buildVAO(GLint mid, bool isTransparent = false) {
        computeDerivedMatrix(PROJ_VIEW_MODEL);
        glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
        glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
        computeNormalMatrix3x3();
        glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);

        if (USE_STENCIL) {
            glStencilFunc(GL_EQUAL, 0x1, 0x1);
            glStencilOp(GL_KEEP, GL_KEEP, GL_KEEP);
        }

        if(isTransparent){
            // TODO: Could this be also obtained from the alpha of material? :)
            glEnable(GL_BLEND);
            glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
            glDepthMask(GL_FALSE); // Don't write to depth buffer=
        }

        //glDisable(GL_BLEND);
        glBindVertexArray(mesh[mid].vao);
        glDrawElements(mesh[mid].type, mesh[mid].numIndexes, GL_UNSIGNED_INT, 0);

        glDepthMask(GL_TRUE);
        glCullFace(GL_BACK);

    }

public:
    Vector3 position;
    bool isTransparent = false;
    GameObject(Vector3 pos) {
        this->position = pos;
    }

    void setEnabled(bool state) {
        this->enabled = state;
    }

    bool isEnabled() {
        return this->enabled;
    }

    bool getAnimationEnabled() {
        return animationEnabled;
    }

    void setAnimationEnabled(bool newAnimationEnabledState) {
        this->animationEnabled = newAnimationEnabledState;
    }

    virtual void init() { }

    virtual void render() { }

    virtual void update(float deltaTime) { }

    // --------------------------------- following methods are meant to be overriden -------------------------------- //
    virtual GameObjectType getType() {
        return UNKNOWN;
    }

    virtual Vector3 getSpeed() {
        return Vector3(0, 0, 0);
    }

    virtual bool collideWith(const GameObject* other) const {
        return false;
    }

    virtual BoundingBox getBoundingBox() const {
        return BoundingBox(Vector3(0, 0, 0), Vector3(0, 0, 0));
    }

    virtual float getSpeedMultiplier() const {
        return -1.0f;
    }

    virtual void setSpeedMultiplier(float newSpeedMult) {
        // Do nothing
    }

    // ---------------------------------- end of the overriden methods ---------------------------------------------- //
};

// Track how many IDs were used
int GameObject::idCount = 0;

#endif //AVT7_GAMEOBJECT_H
