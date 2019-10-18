//
// Created by skalimat on 17.10.19.
//

#ifndef AVT7_GROUND_H
#define AVT7_GROUND_H


class Ground: public GameObject {
public:
    explicit Ground(Vector3 pos): GameObject(pos) {

    }

    void init() override {
        ids.push_back(idCount+=1);
        setMesh(ids.back(), customMaterial.amb, customMaterial.diff_green, customMaterial.spec_matte,
                customMaterial.emissive, customMaterial.shininess, customMaterial.texcount);
        createCube(ids.back());

        ids.push_back(idCount+=1);
        setMesh(ids.back(), customMaterial.amb, customMaterial.diff_green, customMaterial.spec_matte,
                customMaterial.emissive, customMaterial.shininess, customMaterial.texcount);
        createCube(ids.back());

        ids.push_back(idCount+=1);
        setMesh(ids.back(), customMaterial.amb, customMaterial.diff_green, customMaterial.spec_matte,
                customMaterial.emissive, customMaterial.shininess, customMaterial.texcount);
        createCube(ids.back());
    }

    void render() override {
        renderMaterials(ids[0]);
        pushMatrix(MODEL);
        translate(MODEL, position.x, position.y, position.z);
        translate(MODEL, -13 / 2.0, 0, 6 - 0.5);
        scale(MODEL, 13, 1, 1);
        buildVAO(ids[0]);
        popMatrix(MODEL);

        renderMaterials(ids[1]);
        pushMatrix(MODEL);
        translate(MODEL, position.x, position.y, position.z);
        translate(MODEL, -13 / 2.0, 0, 0 - 0.5);
        scale(MODEL, 13, 1, 1);
        buildVAO(ids[1]);
        popMatrix(MODEL);

        renderMaterials(ids[2]);
        pushMatrix(MODEL);
        translate(MODEL, position.x, position.y, position.z);
        translate(MODEL, -13 / 2.0, 0, -6 - 0.5);
        scale(MODEL, 13, 1, 1);
        buildVAO(ids[2]);
        popMatrix(MODEL);
    }
};

#endif //AVT7_GROUND_H
