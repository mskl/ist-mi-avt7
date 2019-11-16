//
// Created by skalimat on 14.11.19.
//

#ifndef AVT7_PARTICLESYSTEM_H
#define AVT7_PARTICLESYSTEM_H

#include "../GameObject.h"
#include "../GameManager.h"

#define MAX_PARTICLES  50

typedef struct {
    float	life;
    float	fade;
    float	r, g, b;
    GLfloat x, y, z;
    GLfloat vx, vy, vz;
    GLfloat ax, ay, az;
} Particle;

class ParticleSystem: public GameObject {
protected:
    int dead_num_particles;
    Particle particles[MAX_PARTICLES];

public:
    ParticleSystem(Vector3 pos)
        : GameObject(pos), dead_num_particles(0) {
        // Do not use the default transparency pipeline!
        isTransparent = true;
    }

    void init() final {
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_particle);
        createQuad(ids.back(), 0.3, 0.3);
    }

    void update(float deltaTime) final {
        float h = 0.3f * deltaTime;
        float g = 4.0f;

        for (int i = 0; i < MAX_PARTICLES; i++) {
            particles[i].x += (h * particles[i].vx);
            particles[i].y += (h * particles[i].vy);
            particles[i].z += (h * particles[i].vz);

            // Simulate the gravity
            particles[i].vy -= h*g;
            particles[i].life -= particles[i].fade;
        }
    }

    void render() final {
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

        // Depth Buffer Read Only
        glDepthMask(GL_FALSE);

        // draw fireworks particles
        renderTexture(texMode_uniformId, PARTICLE_TEXTURE_INDEX);

        for (int i = 0; i < MAX_PARTICLES; i++) {
            if (particles[i].life > 0.0f) {
                mat_particle.diff[3] = particles[i].life;

                setMaterial(ids[0], mat_particle);
                renderMaterials(ids[0]);
                pushMatrix(MODEL);
                    translate(MODEL, particles[i].x, particles[i].y, particles[i].z);
                    buildVAO(ids[0]);
                popMatrix(MODEL);
            } else {
                dead_num_particles++;
            }
        }

        // make depth buffer again writeable
        glDepthMask(GL_TRUE);

        if (dead_num_particles == MAX_PARTICLES) {
            setEnabled(false);
            dead_num_particles = 0;
        }
    }

    void spawnParticles(Vector3 newPos = Vector3(), Vector3 newSpeed = Vector3(1)) {
        setEnabled(true);
        position = newPos;
        GLfloat v, theta, phi;

        for (auto & i : particles) {
            Vector3 ns = Vector3(newSpeed.x, newSpeed.y, newSpeed.z);

            ns.x = ns.x * frand() * 3.0f + (frand()*2-1);
            ns.y = -3.0f*frand();
            ns.z = ns.z * frand() * 3.0f + (frand()*2-1);

            i.x = position.x;
            i.y = position.y;
            i.z = position.z;

            i.vx = ns.x;
            i.vy = ns.y;
            i.vz = ns.z;

            i.life = 1.0f;
            i.fade = 0.05f;
        }
    }

    static float frand() {
        return ((float)rand()/RAND_MAX);
    }
};

#endif //AVT7_PARTICLESYSTEM_H
