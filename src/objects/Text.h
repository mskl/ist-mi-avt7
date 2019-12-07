//
// Created by mykha on 09.11.19.
//

#ifndef AVT7_TEXT_H
#define AVT7_TEXT_H

#include "../GameObject.h"
#include "../GameManager.h"

extern GLint texMode_uniformId;
class Text: public GameObject {
public:

    // TEXT THINGS
    float _fontSize;
    GLuint text_vaoID;
    GLuint text_texCoordBuffer;
    GLuint text_vertexBuffer;
    GLint doingText_uniformId;
    GLint doingTextV_uniformId;
    Text(): GameObject(Vector3(5,0,5)) {

    }

    void init() override {
        ids.push_back(idCount+=1);
        setMaterial(ids.back(), mat_road);
//        /createCube(ids.back());
        initTextureMappedFont();
    }

    /*void render() override {
        pushMatrix(MODEL);
            pushMatrix(MODEL);
                translate(MODEL, 1, 1, 0);
                renderMaterials(ids[0]);
                //translate(MODEL, -13/2, 0, 1);
                //renderTexture(texMode_uniformId, 2);
                scale(MODEL, 13, 1, 5);
                buildVAO(ids[0]);
            popMatrix(MODEL);
        popMatrix(MODEL);
    }*/

    void render() override{

    }
    void initTextureMappedFont() {
        float text_vertices[] = {
                0.0f, 0.0f,
                _fontSize, 0.0f,
                _fontSize, _fontSize,
                0.0f, _fontSize
        };

        glGenVertexArrays(1, &text_vaoID);
        glBindVertexArray(text_vaoID);
        glGenBuffers(1, &text_vertexBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, text_vertexBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, &text_vertices[0], GL_STATIC_DRAW);
        glEnableVertexAttribArray(VERTEX_ATTRIB1);
        glVertexAttribPointer(VERTEX_ATTRIB1, 2, GL_FLOAT, GL_FALSE, 0, 0);

        //Just initialize with something for now, the tex coords are updated
        //for each character printed
        float text_texCoords[] = {
                0.0f, 0.0f,
                0.0f, 0.0f,
                0.0f, 0.0f,
                0.0f, 0.0f
        };

        glGenBuffers(1, &text_texCoordBuffer);
        glBindBuffer(GL_ARRAY_BUFFER, text_texCoordBuffer);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 8, &text_texCoords[0], GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(VERTEX_ATTRIB2);
        glVertexAttribPointer(VERTEX_ATTRIB2, 2, GL_FLOAT, GL_FALSE, 0, 0);

        //set the orthographic projection matrix
        //ortho(0.0f, float(WinX), 0.0f, float(WinY), -1.0f, 1.0f);
    }
};

/**/

#endif //AVT7_ROAD_H
