//
// Created by skalimat on 29.10.19.
//

#ifndef AVT7_SCENESTENCIL_H
#define AVT7_SCENESTENCIL_H

#include "../GameObject.h"

class SceneStencil : public GameObject {
public:
    SceneStencil() : GameObject(Vector3()) {

    }

    void init() override {
        ids.push_back(idCount += 1);
        setMaterial(ids.back(), mat_bus);
        createQuad(ids.back(), 1, 1);
    }

    void render() override {
        int m_viewport[4];
        glGetIntegerv(GL_VIEWPORT, m_viewport);
        float width = m_viewport[2];
        float height = m_viewport[3];

        renderTexture(texMode_uniformId, 0);
        renderMaterials(ids[0]);
        pushMatrix(PROJECTION);
        loadIdentity(PROJECTION);
            pushMatrix(VIEW);
                loadIdentity(VIEW);
                pushMatrix(MODEL);
                    ortho(m_viewport[0], m_viewport[0] + m_viewport[2] - 1, m_viewport[1],
                          m_viewport[1] + m_viewport[3] - 1, -1, 1);
                    translate(MODEL, width/2, height/2, 0);
                    scale(MODEL,     width, height*0.9, 1);
                    if (USE_STENCIL) {
                        glClear(GL_STENCIL_BUFFER_BIT);
                        glStencilFunc(GL_NEVER, 0x1, 0x1);    // Always fails
                        glStencilOp(GL_REPLACE, GL_KEEP, GL_KEEP);      // actions to change stencil buffer
                    }
                    buildVAO(ids[0]);
                popMatrix(MODEL);
            popMatrix(VIEW);
        popMatrix(PROJECTION);
    }
};

#endif //AVT7_SCENESTENCIL_H
