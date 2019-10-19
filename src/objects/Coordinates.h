//
// Created by skalimat on 18.10.19.
//

#ifndef AVT7_COORDINATES_H
#define AVT7_COORDINATES_H

class Coordinates: public GameObject {
public:

    Coordinates(Vector3 pos): GameObject(pos) {

    }

    void init() override {
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_coord_x);
        createCube(ids.back());

        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_coord_y);
        createCube(ids.back());

        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_coord_z);
        createCube(ids.back());
    }

    void render() override {
        pushMatrix(MODEL);
            translate(MODEL, position.x , position.y, position.z);
            renderMaterials(ids[0]);
            pushMatrix(MODEL);
                translate(MODEL, -50, 0, 0);
                scale(MODEL, 100, 0.01, 0.01);
                buildVAO(ids[0]);
            popMatrix(MODEL);

            renderMaterials(ids[1]);
            pushMatrix(MODEL);
                translate(MODEL, 0, -50, 0);
                scale(MODEL, 0.01, 100, 0.01);
                buildVAO(ids[1]);
            popMatrix(MODEL);

            renderMaterials(ids[2]);
            pushMatrix(MODEL);
                translate(MODEL, 0, 0, -50);
                scale(MODEL, 0.01, 0.01, 100);
                buildVAO(ids[2]);
            popMatrix(MODEL);
        popMatrix(MODEL);
    }
};

#endif //AVT7_COORDINATES_H
