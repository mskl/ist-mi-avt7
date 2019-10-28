//
// Created by skalimat on 27.10.19.
//

#ifndef AVT7_VEHICLESPAWNER_H
#define AVT7_VEHICLESPAWNER_H

#include "../GameObject.h"

template <typename T>
class VehicleSpawner: public GameObject {
public:
    Vector3 spawnDirection;

    VehicleSpawner(Vector3 pos, Vector3 dir, float minDelay, float maxDelay): GameObject(pos), spawnDirection(dir) {

    }
};

#endif //AVT7_VEHICLESPAWNER_H
