//
// Created by skalimat on 27.10.19.
//

#ifndef AVT7_VEHICLESPAWNER_H
#define AVT7_VEHICLESPAWNER_H

#include <queue>
#include "../GameObject.h"

template <typename T>
class VehicleSpawner: public GameObject {
    Vector3 spawnDirection;
    int minDelay;
    int lastSpawnedTime;
    int randomDelay;
    queue<T> vehicleQueue = queue<T>();
public:

    VehicleSpawner(Vector3 pos, Vector3 dir, int minDelay)
        : GameObject(pos), spawnDirection(dir), minDelay(minDelay) {
        // Random delay maximum 1000ms
        randomDelay = rand() % 1000;
    }

    void initQueue(int n_objects) {
        for (int i = 0; i < n_objects; i++) {
            vehicleQueue.push(new T());
        }
    }

    void update(int deltaTime) override {
        GLint currentTime = glutGet(GLUT_ELAPSED_TIME);

        if (vehicleQueue.size() > 0) {
            if (currentTime - lastSpawnedTime < minDelay + randomDelay) {
                // Spawn the vehicle
                lastSpawnedTime = currentTime;
                randomDelay = random() % 1000;

                T* spawnedVehicle = vehicleQueue.pop();
                // Enable the GameObject
                spawnedVehicle->setEnabled(true);
                // Set the position of the GO
                spawnedVehicle->position = this->position;
            }
        }
    }

    void addVehicle(T* vehicle) {
        // Disable the GameObject
        vehicle->setEnabled(false);

        // Push back to the queue
        vehicleQueue.push(vehicle);
    }
};

#endif //AVT7_VEHICLESPAWNER_H
