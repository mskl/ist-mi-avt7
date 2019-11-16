//
// Created by skalimat on 14.11.19.
//

#ifndef AVT7_PARTICLESYSTEM_H
#define AVT7_PARTICLESYSTEM_H

#include "../GameObject.h"
#include "../GameManager.h"

#define MAX_PARTICLES  30

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
        createQuad(ids.back(), 2, 2);
        // createCube(ids.back(), true);
    }

    void update(float deltaTime) final {
        float h = 0.3 * deltaTime;
        for (int i = 0; i < MAX_PARTICLES; i++) {
            particles[i].x += (h * particles[i].vx);
            particles[i].y += (h * particles[i].vy);
            particles[i].z += (h * particles[i].vz);
            particles[i].vx += (h * particles[i].ax);
            particles[i].vy += (h * particles[i].ay);
            particles[i].vz += (h * particles[i].az);
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
                mat_particle.diff[0] = particles[i].r;
                mat_particle.diff[1] = particles[i].g;
                mat_particle.diff[2] = particles[i].b;
                mat_particle.diff[3] = particles[i].life;

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

    void spawnParticles(Vector3 newPos = Vector3()) {
        setEnabled(true);
        position = newPos;
        GLfloat v, theta, phi;

        for (auto & i : particles) {
            v = 0.8 * frand() + 0.2;
            phi = frand() * M_PI;
            theta = 2.0 * frand() *M_PI;

            i.x = position.x;
            i.y = position.y;
            i.z = position.z;
            i.vx = v * cos(theta) * sin(phi);
            i.vy = v * cos(phi);
            i.vz = v * sin(theta) * sin(phi);
            i.ax = 0.1f;    /* simular um pouco de vento */
            i.ay = -0.15f;  /* simular a acelera��o da gravidade */
            i.az = 0.0f;

            /* tom amarelado que vai ser multiplicado pela textura que varia entre branco e preto */
            i.r = 0.882f;
            i.g = 0.552f;
            i.b = 0.211f;

            i.life = 1.0f;
            i.fade = 0.005f;
        }
    }

    static float frand() {
        return ((float)rand()/RAND_MAX);
    }
};

#endif //AVT7_PARTICLESYSTEM_H
