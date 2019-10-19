//
// Created by skalimat on 19.10.19.
//

#ifndef AVT7_AMATERIAL_H
#define AVT7_AMATERIAL_H

struct AMaterial {
    float* amb;
    float* diff;
    float* spec;
    float* emissive;
    float shininess;
    int texcount;

    AMaterial(float* n_amb, float* n_diff, float* n_spec, float* n_emmisive, float n_shininess, int n_texCount) {
        amb = n_amb;
        diff = n_diff;
        spec = n_spec;
        emissive = n_emmisive;
        shininess = n_shininess;
        texcount = n_texCount;
    }
};

#endif //AVT7_AMATERIAL_H
