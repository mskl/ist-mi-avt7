//
// Created by skalimat on 17.10.19.
//

#ifndef AVT7_GROUND_H
#define AVT7_GROUND_H


class Ground: public CollidableGameObject {
public:
    explicit Ground(Vector3 pos, Vector3 maxPos): CollidableGameObject(pos, Vector3(0,0,0), maxPos, GROUND) {

    }

    void init() override {
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_ground);
        createCube(ids.back());

        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_ground);
        createCube(ids.back());

        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_ground);
        createCube(ids.back());

        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_ground);
        createCube(ids.back());

    }

    void render() override {
        renderTexture(texMode_uniformId, 2);
        pushMatrix(MODEL);
            // The integral division by 2 is wanted
            pushMatrix(MODEL);
                renderMaterials(ids[0]);
                translate(MODEL, position.x, position.y, position.z);
                scale(MODEL, boundingBox.vecMax.x,boundingBox.vecMax.y,boundingBox.vecMax.z);
                buildVAO(ids[0]);
            popMatrix(MODEL);
            /*
            pushMatrix(MODEL);
                renderMaterials(ids[1]);
                translate(MODEL, -13 / 2, 0, 0);
                scale(MODEL, 13, 1, 1);
                buildVAO(ids[1]);
            popMatrix(MODEL);
            //Other side
            pushMatrix(MODEL);
                renderMaterials(ids[2]);
                translate(MODEL, -13 / 2, 0, -6);
                scale(MODEL, 1, 1, 1);
                buildVAO(ids[2]);
            popMatrix(MODEL);

            pushMatrix(MODEL);
                renderMaterials(ids[2]);
                translate(MODEL, -4, 0, -6);
                scale(MODEL, 1, 1, 1);
                buildVAO(ids[2]);
            popMatrix(MODEL);

            pushMatrix(MODEL);
                renderMaterials(ids[2]);
                translate(MODEL, -2, 0, -6);
                scale(MODEL, 1, 1, 1);
                buildVAO(ids[2]);
            popMatrix(MODEL);

            pushMatrix(MODEL);
                renderMaterials(ids[2]);
                translate(MODEL, 0, 0, -6);
                scale(MODEL, 1, 1, 1);
                buildVAO(ids[2]);
            popMatrix(MODEL);

            pushMatrix(MODEL);
                renderMaterials(ids[2]);
                translate(MODEL, 2, 0, -6);
                scale(MODEL, 1, 1, 1);
                buildVAO(ids[2]);
            popMatrix(MODEL);
            pushMatrix(MODEL);
                renderMaterials(ids[2]);
                translate(MODEL, 4, 0, -6);
                scale(MODEL, 1, 1, 1);
                buildVAO(ids[2]);
            popMatrix(MODEL);
            pushMatrix(MODEL);
                renderMaterials(ids[2]);
                translate(MODEL, 6, 0, -6);
                scale(MODEL, 1, 1, 1);
                buildVAO(ids[2]);
            popMatrix(MODEL);
            */
        popMatrix(MODEL);
    }
};

#endif //AVT7_GROUND_H
