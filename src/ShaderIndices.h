//
// Created by skalimat on 18.10.19.
//

#include <GL/glew.h>

#ifndef AVT7_SHADERINDICES_H
#define AVT7_SHADERINDICES_H

struct ShaderIndices {
    ShaderIndices(GLint l_amb, GLint l_dif, GLint l_spc, GLint l_shi, GLint l_cnt)
        : loc_amb(l_amb), loc_dif(l_dif), loc_spc(l_spc), loc_shi(l_shi), loc_txcnt(l_cnt) { }

    GLint loc_amb;
    GLint loc_dif;
    GLint loc_spc;
    GLint loc_shi;
    GLint loc_txcnt;
};

#endif //AVT7_SHADERINDICES_H
