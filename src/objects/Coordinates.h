//
// Created by skalimat on 18.10.19.
//

#ifndef AVT7_COORDINATES_H
#define AVT7_COORDINATES_H

class Coordinates: public GameObject {
public:

    Coordinates(Vector3 pos, int id): GameObject(pos, id) {

    }

    void init() override {
        objId = 666;
        memcpy(mesh[objId].mat.ambient, customMaterial.amb, 4 * sizeof(float));
        memcpy(mesh[objId].mat.diffuse, customMaterial.diff_blue, 4 * sizeof(float));
        memcpy(mesh[objId].mat.specular, customMaterial.spec_matte, 4 * sizeof(float));
        memcpy(mesh[objId].mat.emissive, customMaterial.emissive, 4 * sizeof(float));
        mesh[objId].mat.shininess = customMaterial.shininess;
        mesh[objId].mat.texCount = customMaterial.texcount;
        createCube(666);

        objId = 667;
        memcpy(mesh[objId].mat.ambient, customMaterial.amb, 4 * sizeof(float));
        memcpy(mesh[objId].mat.diffuse, customMaterial.diff_yellow, 4 * sizeof(float));
        memcpy(mesh[objId].mat.specular, customMaterial.spec_matte, 4 * sizeof(float));
        memcpy(mesh[objId].mat.emissive, customMaterial.emissive, 4 * sizeof(float));
        mesh[objId].mat.shininess = customMaterial.shininess;
        mesh[objId].mat.texCount = customMaterial.texcount;
        createCube(667);
        objId = 668;
        memcpy(mesh[objId].mat.ambient, customMaterial.amb, 4 * sizeof(float));
        memcpy(mesh[objId].mat.diffuse, customMaterial.diff_green, 4 * sizeof(float));
        memcpy(mesh[objId].mat.specular, customMaterial.spec_matte, 4 * sizeof(float));
        memcpy(mesh[objId].mat.emissive, customMaterial.emissive, 4 * sizeof(float));
        mesh[objId].mat.shininess = customMaterial.shininess;
        mesh[objId].mat.texCount = customMaterial.texcount;
        createCube(668);
    }

    void render() override {
        objId = 666;
        GLint loc_amb = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
        glUniform4fv(loc_amb, 1, mesh[objId].mat.ambient);
        GLint loc_dif = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
        glUniform4fv(loc_dif, 1, mesh[objId].mat.diffuse);
        GLint loc_spc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
        glUniform4fv(loc_spc, 1, mesh[objId].mat.specular);
        GLint loc_shi = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
        glUniform1f(loc_shi, mesh[objId].mat.shininess);
        pushMatrix(MODEL);
        translate(MODEL, position.x - 50, position.y, position.z);
        scale(MODEL, 100, 0.01, 0.01);
        computeDerivedMatrix(PROJ_VIEW_MODEL);
        glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
        glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
        computeNormalMatrix3x3();
        glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
        glBindVertexArray(mesh[objId].vao);
        glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        popMatrix(MODEL);

        objId = 667;
        loc_amb = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
        glUniform4fv(loc_amb, 1, mesh[objId].mat.ambient);
        loc_dif = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
        glUniform4fv(loc_dif, 1, mesh[objId].mat.diffuse);
        loc_spc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
        glUniform4fv(loc_spc, 1, mesh[objId].mat.specular);
        loc_shi = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
        glUniform1f(loc_shi, mesh[objId].mat.shininess);
        pushMatrix(MODEL);
        translate(MODEL, position.x, position.y - 50, position.z);
        scale(MODEL, 0.01, 100, 0.01);
        computeDerivedMatrix(PROJ_VIEW_MODEL);
        glUniformMatrix4fv(vm_uniformId, 1, GL_FALSE, mCompMatrix[VIEW_MODEL]);
        glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);
        computeNormalMatrix3x3();
        glUniformMatrix3fv(normal_uniformId, 1, GL_FALSE, mNormal3x3);
        glBindVertexArray(mesh[objId].vao);
        glDrawElements(mesh[objId].type, mesh[objId].numIndexes, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        popMatrix(MODEL);

        objId = 668;
        loc_amb = glGetUniformLocation(shader.getProgramIndex(), "mat.ambient");
        glUniform4fv(loc_amb, 1, mesh[objId].mat.ambient);
        loc_dif = glGetUniformLocation(shader.getProgramIndex(), "mat.diffuse");
        glUniform4fv(loc_dif, 1, mesh[objId].mat.diffuse);
        loc_spc = glGetUniformLocation(shader.getProgramIndex(), "mat.specular");
        glUniform4fv(loc_spc, 1, mesh[objId].mat.specular);
        loc_shi = glGetUniformLocation(shader.getProgramIndex(), "mat.shininess");
        glUniform1f(loc_shi, mesh[objId].mat.shininess);
        pushMatrix(MODEL);
        translate(MODEL, position.x, position.y, position.z - 50);
        scale(MODEL, 0.01, 0.01, 100);
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

#endif //AVT7_COORDINATES_H
