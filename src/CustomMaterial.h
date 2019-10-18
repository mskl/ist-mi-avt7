//
// Created by skalimat on 17.10.19.
//

#ifndef AVT7_CUSTOMMATERIAL_H
#define AVT7_CUSTOMMATERIAL_H

struct CustomMaterial{
    float amb[4] = {0.2f, 0.15f, 0.1f, 1.0f};
    float amb_green[4] = {0.0f, 1.0f, 0.0f, 1.0f};
    float amb_blue[4] = {0.0f, 0.0f, 1.0f, 1.0f};
    float amb_yellow[4] = {1.0f, 1.0f, 0.0f, 0.0f};


    float diff[4] = {0.8f, 0.6f, 0.4f, 1.0f};
    float diff_black[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    float diff_yellow[4] = {1.0f, 1.0f, 0.0f, 1.0f};
    float diff_blue[4] = {0.0f, 0.0f, 1.0f, 1.0f};
    float diff_green[4] = {0.0f, 1.0f, 0.0f, 1.0f};
    float diff_red[4] = {1.0f, 0.0f, 0.0f, 1.0f};

    float spec[4] = {0.8f, 0.8f, 0.8f, 1.0f};
    float spec_matte[4] = {0, 0, 0, 1};

    float emissive[4] = {0.0f, 0.0f, 0.0f, 1.0f};
    float emit_green[4] = {0.0f, 1.0f, 0.0f, 1.0f};
    float emit_blue[4] = {0.0f, 0.0f, 1.0f, 1.0f};
    float emit_yellow[4] = {1.0f, 1.0f, 0.0f, 0.0f};


    float shininess=10.0f;
    int texcount = 0;

}; CustomMaterial customMaterial;

#endif //AVT7_CUSTOMMATERIAL_H
