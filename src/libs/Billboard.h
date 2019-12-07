#ifndef AVT7_BILLBOARD_H
#define AVT7_BILLBOARD_H

#include <math.h>
#include "AVTmathLib.h"
#include <cstring>

/* Computes the inner product between vectors v and q */
#define mathsInnerProduct(v, q) \
    ((v)[0] * (q)[0] + \
    (v)[1] * (q)[1] + \
    (v)[2] * (q)[2])

/* a = b x c */
#define mathsCrossProduct(a, b, c) \
    (a)[0] = (b)[1] * (c)[2] - (c)[1] * (b)[2]; \
    (a)[1] = (b)[2] * (c)[0] - (c)[2] * (b)[0]; \
    (a)[2] = (b)[0] * (c)[1] - (c)[0] * (b)[1];

inline void mathsNormalize(float *v) {
    float d = (sqrt((v[0]*v[0]) + (v[1]*v[1]) + (v[2]*v[2])));
    v[0] = v[0] / d;
    v[1] = v[1] / d;
    v[2] = v[2] / d;
}

void l3dBillboardSphericalBegin(float *cam, float *worldPos) {
    float lookAt[3] = {0, 0, 1}, objToCamProj[3], objToCam[3], upAux[3], angleCosine;

    // objToCamProj is the vector in world coordinates from the local origin to the camera
    // projected in the XZ plane
    objToCamProj[0] = cam[0] - worldPos[0];
    objToCamProj[1] = 0;
    objToCamProj[2] = cam[2] - worldPos[2];

    // normalize both vectors to get the cosine directly afterwards
    mathsNormalize(objToCamProj);

    // easy fix to determine wether the angle is negative or positive
    // for positive angles upAux will be a vector pointing in the
    // positive y direction, otherwise upAux will point downwards
    // effectively reversing the rotation.

    mathsCrossProduct(upAux, lookAt, objToCamProj);

    // compute the angle
    angleCosine = mathsInnerProduct(lookAt, objToCamProj);

    // perform the rotation. The if statement is used for stability reasons
    // if the lookAt and v vectors are too close together then |aux| could
    // be bigger than 1 due to lack of precision
    if ((angleCosine < 0.99990) && (angleCosine > -0.9999))
        rotate(MODEL, acos(angleCosine) * 180 / 3.14, upAux[0], upAux[1], upAux[2]);


    // The second part tilts the object so that it faces the camera

    // objToCam is the vector in world coordinates from the local origin to the camera
    objToCam[0] = cam[0] - worldPos[0];
    objToCam[1] = cam[1] - worldPos[1];
    objToCam[2] = cam[2] - worldPos[2];

    // Normalize to get the cosine afterwards
    mathsNormalize(objToCam);

    // Compute the angle between v and v2, i.e. compute the
    // required angle for the lookup vector
    angleCosine = mathsInnerProduct(objToCamProj, objToCam);


    // Tilt the object. The test is done to prevent instability when objToCam and objToCamProj have a very small
    // angle between them
    if ((angleCosine < 0.99990) && (angleCosine > -0.9999))
        if (objToCam[1] < 0)
            rotate(MODEL, acos(angleCosine) * 180 / 3.14, 1, 0, 0);
        else
            rotate(MODEL, acos(angleCosine) * 180 / 3.14, -1, 0, 0);

}

#endif //AVT7_BILLBOARD_H
