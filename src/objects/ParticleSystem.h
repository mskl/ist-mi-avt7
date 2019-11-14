//
// Created by skalimat on 14.11.19.
//

#ifndef AVT7_PARTICLESYSTEM_H
#define AVT7_PARTICLESYSTEM_H

#include "../GameObject.h"
#include "../GameManager.h"

#define MAX_PARTICULAS  150
#define frand()			((float)rand()/RAND_MAX)

typedef struct {
    float	life;
    float	fade;
    float	r, g, b;
    GLfloat x, y, z;
    GLfloat vx, vy, vz;
    GLfloat ax, ay, az;
} Particle;

class ParticleSystem: public GameObject {
public:
    Particle particula[MAX_PARTICULAS];
    int dead_num_particles = 0;

    ParticleSystem(Vector3 pos) : GameObject(pos) {

    }

    void init() final {
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_ground);
        createQuad(ids.back(), 2, 2);
    }

    void update(float deltaTime) final {
        float h = 0.033 * deltaTime;
        if (isEnabled()) {
            for (int i = 0; i < MAX_PARTICULAS; i++) {
                particula[i].x += (h*particula[i].vx);
                particula[i].y += (h*particula[i].vy);
                particula[i].z += (h*particula[i].vz);
                particula[i].vx += (h*particula[i].ax);
                particula[i].vy += (h*particula[i].ay);
                particula[i].vz += (h*particula[i].az);
                particula[i].life -= particula[i].fade;
            }
            // glutPostRedisplay();
            // glutTimerFunc(33, iterate, 0);
        }
    }

    void render() final {
        // draw fireworks particles
        renderTexture(texMode_uniformId, PARTICLE_TEXTURE_INDEX);

        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDepthMask(GL_FALSE);  // Depth Buffer Read Only

/*
        glBindTexture(GL_TEXTURE_2D, TextureArray[PARTICLE_TEXTURE_INDEX]); //particle.tga associated to TU0
        // GL STUFF WAS HERE
        glUniform1i(texMode_uniformId, PARTICLE_TEXTURE_INDEX); // draw modulated textured particles
*/

        for (int i = 0; i < MAX_PARTICULAS; i++) {
            if (particula[i].life > 0.0f) {
                // TODO: Change the color of the material
                /*
                float particle_color[4];
                particle_color[0] = particula[i].r;
                particle_color[1] = particula[i].g;
                particle_color[2] = particula[i].b;
                particle_color[3] = particula[i].life;
                */
                renderMaterials(ids[0]);
                pushMatrix(MODEL);
                translate(MODEL, particula[i].x, particula[i].y, particula[i].z);
                buildVAO(ids[0]);
                popMatrix(MODEL);
            } else {
                dead_num_particles++;
            }
        }

        glDepthMask(GL_TRUE); //make depth buffer again writeable

        if (dead_num_particles == MAX_PARTICULAS) {
            setEnabled(false);
            dead_num_particles = 0;
            printf("All particles dead\n");
        }
    }

    void initParticles(Vector3 newPos = Vector3()) {
        setEnabled(true);
        position = newPos;
        GLfloat v, theta, phi;

        for (auto & i : particula) {
            v = 0.8 * frand() + 0.2;
            phi = frand() * M_PI;
            theta = 2.0* frand() *M_PI;

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

            i.life = 1.0f;		/* vida inicial */
            i.fade = 0.005f;    /* step de decr�scimo da vida para cada itera��o */
        }
    }
};

#endif //AVT7_PARTICLESYSTEM_H
