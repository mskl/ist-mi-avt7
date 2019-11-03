/* --------------------------------------------------
Basic Revolution Geometry 
 *
 * based on vsResourceLib.cpp from Lighthouse3D
 *
 *
 *
 * Full documentation at 
 * http://www.lighthouse3d.com/very-simple-libs

 Jo?o Madeiras Pereira
----------------------------------------------------*/
#include <assert.h>
#include <stdlib.h>
#include <vector>
#include <cstring>
#include <math.h>

// include GLEW to access OpenGL 3.3 functions
#include <GL/glew.h>

// GLUT is the toolkit to interface with the OS
#include <GL/freeglut.h>

#include "../VertexAttrDef.h"
#include "basic_geometry.h"
#include "../primitives/cube.h"

extern struct MyMesh mesh[];
GLuint VboId[2];

/*
void createQuad(float size_x, float size_y)
{
	int i;
	float vert[16];
	mesh[objId].numIndexes = 2*3;

	memcpy(vert, quad_vertices, sizeof(float) * 16);

	for(i=0; i< 4; i++) {
		vert[i*4] *= size_x;
		vert[i*4+1] *= size_y;
	}

	glGenVertexArrays(1, &(mesh[objId].vao));
	glBindVertexArray(mesh[objId].vao);

	glGenBuffers(2, VboId);
	glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_vertices)+sizeof(quad_normals)+sizeof(quad_texCoords),NULL,GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(quad_vertices), vert);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(quad_vertices), sizeof(quad_normals), quad_normals);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(quad_vertices)+ sizeof(quad_normals), sizeof(quad_texCoords), quad_texCoords);
    
    glEnableVertexAttribArray(VERTEX_COORD_ATTRIB);
    glVertexAttribPointer(VERTEX_COORD_ATTRIB, 4, GL_FLOAT, 0, 0, 0);
    glEnableVertexAttribArray(NORMAL_ATTRIB);
    glVertexAttribPointer(NORMAL_ATTRIB, 4, GL_FLOAT, 0, 0, (void *)sizeof(quad_vertices));
    glEnableVertexAttribArray(TEXTURE_COORD_ATTRIB);
    glVertexAttribPointer(TEXTURE_COORD_ATTRIB, 4, GL_FLOAT, 0, 0, (void *)(sizeof(quad_vertices)+ sizeof(quad_normals)));
    
    //index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh[objId].numIndexes, quad_faceIndex , GL_STATIC_DRAW);
    
    // unbind the VAO
    glBindVertexArray(0);

	mesh[objId].type = GL_TRIANGLES;
}
*/

void createCubeMinMax(GLint _objId, Vector3 _min, Vector3 _max) {
    // Works only for one object in scene, acts weird
    mesh[_objId].numIndexes = faceCount * 3;

    glGenVertexArrays(1, &(mesh[_objId].vao));
    glBindVertexArray(mesh[_objId].vao);

    glGenBuffers(2, VboId);
    glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals) + sizeof(texCoords), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), get_shifted_vertices(_min, _max));
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(normals), normals);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals), sizeof(texCoords), texCoords);

    glEnableVertexAttribArray(VERTEX_COORD_ATTRIB);
    glVertexAttribPointer(VERTEX_COORD_ATTRIB, 4, GL_FLOAT, 0, 0, 0);
    glEnableVertexAttribArray(NORMAL_ATTRIB);
    glVertexAttribPointer(NORMAL_ATTRIB, 4, GL_FLOAT, 0, 0, (void *) sizeof(vertices));
    glEnableVertexAttribArray(TEXTURE_COORD_ATTRIB);
    glVertexAttribPointer(TEXTURE_COORD_ATTRIB, 4, GL_FLOAT, 0, 0, (void *) (sizeof(vertices) + sizeof(normals)));

    //index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh[_objId].numIndexes, faceIndex, GL_STATIC_DRAW);

    // unbind the VAO
    glBindVertexArray(0);

    mesh[_objId].type = GL_TRIANGLES;
}

void createCube(GLint _objId) {
    mesh[_objId].numIndexes = faceCount * 3;

    glGenVertexArrays(1, &(mesh[_objId].vao));
    glBindVertexArray(mesh[_objId].vao);

    glGenBuffers(2, VboId);
    glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals) + sizeof(texCoords), NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices), sizeof(normals), normals);
    glBufferSubData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals), sizeof(texCoords), texCoords);

    glEnableVertexAttribArray(VERTEX_COORD_ATTRIB);
    glVertexAttribPointer(VERTEX_COORD_ATTRIB, 4, GL_FLOAT, 0, 0, 0);
    glEnableVertexAttribArray(NORMAL_ATTRIB);
    glVertexAttribPointer(NORMAL_ATTRIB, 4, GL_FLOAT, 0, 0, (void *) sizeof(vertices));
    glEnableVertexAttribArray(TEXTURE_COORD_ATTRIB);
    glVertexAttribPointer(TEXTURE_COORD_ATTRIB, 4, GL_FLOAT, 0, 0, (void *) (sizeof(vertices) + sizeof(normals)));


    //index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh[_objId].numIndexes, faceIndex, GL_STATIC_DRAW);

    // unbind the VAO
    glBindVertexArray(0);

    mesh[_objId].type = GL_TRIANGLES;
}

void computeVAOSquare(float* p, GLint _objId)
{
    GLuint faceIndex[] = {
            0, 1, 2, 0, 2, 3,
            4, 5, 6, 4, 6, 7,
            8, 9, 10, 8, 10, 11,
            12, 13, 14, 12, 14, 15,
            16, 17, 18, 16, 18, 19,
            20, 21, 22, 20, 22, 23
    };

    float texCoords[] = {
            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,

            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,

            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,

            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,

            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,

            0.0f, 1.0f,
            0.0f, 0.0f,
            1.0f, 0.0f,
            1.0f, 1.0f,
    };

    float normals[] = {
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 1.0f, 0.0f,

            0.0f, 0.0f, -1.0f, 0.0f,
            0.0f, 0.0f, -1.0f, 0.0f,
            0.0f, 0.0f, -1.0f, 0.0f,
            0.0f, 0.0f, -1.0f, 0.0f,

            1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f, 0.0f,
            1.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,
            0.0f, 1.0f, 0.0f, 0.0f,

            -1.0f, 0.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f, 0.0f,
            -1.0f, 0.0f, 0.0f, 0.0f,

            0.0f, -1.0f, 0.0f, 0.0f,
            0.0f, -1.0f, 0.0f, 0.0f,
            0.0f, -1.0f, 0.0f, 0.0f,
            0.0f, -1.0f, 0.0f, 0.0f,
    };

    glGenVertexArrays(1, &mesh[_objId].vao);
    glBindVertexArray(mesh[_objId].vao);

    mesh[_objId].numIndexes = sizeof(faceIndex);
    GLuint buffers[4];
    glGenBuffers(4, buffers);
    //vertex coordinates buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float) * 24 * 4, p, GL_STATIC_DRAW);
    glEnableVertexAttribArray(VERTEX_COORD_ATTRIB);
    glVertexAttribPointer(VERTEX_COORD_ATTRIB, 4, GL_FLOAT, 0, 0, 0);

    //texture coordinates buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(TEXTURE_COORD_ATTRIB);
    glVertexAttribPointer(TEXTURE_COORD_ATTRIB, 2, GL_FLOAT, 0, 0, 0);


    //normals buffer
    glBindBuffer(GL_ARRAY_BUFFER, buffers[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(NORMAL_ATTRIB);
    glVertexAttribPointer(NORMAL_ATTRIB, 4, GL_FLOAT, GL_FALSE, 0, 0);

    //index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, buffers[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mesh[_objId].numIndexes, faceIndex, GL_STATIC_DRAW);

    //Unbind the VAO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glDisableVertexAttribArray(VERTEX_COORD_ATTRIB);
    glDisableVertexAttribArray(NORMAL_ATTRIB);
    glDisableVertexAttribArray(TEXTURE_COORD_ATTRIB);

    mesh[_objId].type = GL_TRIANGLES;
}

void createRectangle(float length, float height, float width, GLint mid){
    float p[] = {
            0.0f, height, width, 1.0f,
            0.0f, 0.0f, width, 1.0f,
            length, 0.0f, width, 1.0f,
            length, height, width, 1.0f,

            length, height, 0.0f, 1.0f,
            length, 0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
            0.0f, height, 0.0f, 1.0f,

            length, height, width, 1.0f,
            length, 0.0f, width, 1.0f,
            length, 0.0f, 0.0f, 1.0f,
            length, height, 0.0, 1.0f,

            0.0f, height, 0.0f, 1.0f,
            0.0f, height, width, 1.0f,
            length, height, width, 1.0f,
            length, height, 0.0f, 1.0f,

            0.0f, height, 0.0f, 1.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
            0.0f, 0.0f, width, 1.0f,
            0.0f, height, width, 1.0f,

            0.0f, 0.0f, width, 1.0f,
            0.0f, 0.0f, 0.0f, 1.0f,
            length, 0.0f, 0.0f, 1.0f,
            length, 0.0f, width, 1.0f,
    };

    computeVAOSquare(p, mid);
}

void createCube(float size, GLint mid)
{
    createRectangle(size, size, size, mid);
}

/*
void createSphere(float radius, int divisions) {
    float *p = circularProfile(-3.14159f / 2.0f, 3.14159f / 2.0f, radius, divisions);
    computeVAO(divisions + 1, p + 2, p, divisions * 2, 0.0f);
}

void computeVAO(int numP, float *p, float *points, int sides, float smoothCos) {
    // Compute and store vertices

    int numSides = sides;
    int numPoints = numP + 2;

    float *vertex = (float *) malloc(sizeof(float) * numP * 2 * 4 * (numSides + 1));
    float *normal = (float *) malloc(sizeof(float) * numP * 2 * 4 * (numSides + 1));
    float *textco = (float *) malloc(sizeof(float) * numP * 2 * 4 * (numSides + 1));


    float inc = 2 * 3.14159f / (numSides);
    float nx, ny;
    float delta;
    int smooth;
    std::vector<int> smoothness;
    int k = 0;
    for (int i = 0; i < numP; i++) {
        revSmoothNormal2(points + (i * 2), &nx, &ny, smoothCos, 0);
        for (int j = 0; j <= numSides; j++) {

            if ((i == 0 && p[0] == 0.0f) || (i == numP - 1 && p[(i + 1) * 2] == 0.0))
                delta = inc * 0.5f;
            else
                delta = 0.0f;

            normal[((k) * (numSides + 1) + j) * 4] = nx * cos(j * inc + delta);
            normal[((k) * (numSides + 1) + j) * 4 + 1] = ny;
            normal[((k) * (numSides + 1) + j) * 4 + 2] = nx * sin(-j * inc + delta);
            normal[((k) * (numSides + 1) + j) * 4 + 3] = 0.0f;

            vertex[((k) * (numSides + 1) + j) * 4] = p[i * 2] * cos(j * inc);
            vertex[((k) * (numSides + 1) + j) * 4 + 1] = p[(i * 2) + 1];
            vertex[((k) * (numSides + 1) + j) * 4 + 2] = p[i * 2] * sin(-j * inc);
            vertex[((k) * (numSides + 1) + j) * 4 + 3] = 1.0f;

            textco[((k) * (numSides + 1) + j) * 4] = ((j + 0.0f) / numSides);
            textco[((k) * (numSides + 1) + j) * 4 + 1] = (i + 0.0f) / (numP - 1);
            textco[((k) * (numSides + 1) + j) * 4 + 2] = 0;
            textco[((k) * (numSides + 1) + j) * 4 + 3] = 1.0f;
        }
        k++;
        if (i < numP - 1) {
            smooth = revSmoothNormal2(points + ((i + 1) * 2), &nx, &ny, smoothCos, 1);

            if (!smooth) {
                smoothness.push_back(1);
                for (int j = 0; j <= numSides; j++) {

                    normal[((k) * (numSides + 1) + j) * 4] = nx * cos(j * inc);
                    normal[((k) * (numSides + 1) + j) * 4 + 1] = ny;
                    normal[((k) * (numSides + 1) + j) * 4 + 2] = nx * sin(-j * inc);
                    normal[((k) * (numSides + 1) + j) * 4 + 3] = 0.0f;

                    vertex[((k) * (numSides + 1) + j) * 4] = p[(i + 1) * 2] * cos(j * inc);
                    vertex[((k) * (numSides + 1) + j) * 4 + 1] = p[((i + 1) * 2) + 1];
                    vertex[((k) * (numSides + 1) + j) * 4 + 2] = p[(i + 1) * 2] * sin(-j * inc);
                    vertex[((k) * (numSides + 1) + j) * 4 + 3] = 1.0f;

                    textco[((k) * (numSides + 1) + j) * 4] = ((j + 0.0f) / numSides);
                    textco[((k) * (numSides + 1) + j) * 4 + 1] = (i + 1 + 0.0f) / (numP - 1);
                    textco[((k) * (numSides + 1) + j) * 4 + 2] = 0;
                    textco[((k) * (numSides + 1) + j) * 4 + 3] = 1.0f;
                }
                k++;
            } else
                smoothness.push_back(0);
        }
    }

    unsigned int *faceIndex = (unsigned int *) malloc(sizeof(unsigned int) * (numP - 1) * (numSides + 1) * 6);
    unsigned int count = 0;
    k = 0;
    for (int i = 0; i < numP - 1; ++i) {
        for (int j = 0; j < numSides; ++j) {

            if (i != 0 || p[0] != 0.0) {
                faceIndex[count++] = k * (numSides + 1) + j;
                faceIndex[count++] = (k + 1) * (numSides + 1) + j + 1;
                faceIndex[count++] = (k + 1) * (numSides + 1) + j;
            }
            if (i != numP-2 || p[(numP-1)*2] != 0.0) {
                faceIndex[count++] = k * (numSides + 1) + j;
                faceIndex[count++] = k * (numSides + 1) + j + 1;
                faceIndex[count++] = (k + 1) * (numSides + 1) + j + 1;
            }

        }
        k++;
        k += smoothness[i];
    }

    int numVertices = numP * 2 * (numSides + 1);
    mesh[objId].numIndexes = count;

    glGenVertexArrays(1, &(mesh[objId].vao));
    glBindVertexArray(mesh[objId].vao);

    //Implementation with glBufferSubData just for learning purposes

    glGenBuffers(2, VboId);
    glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
    glBufferData(GL_ARRAY_BUFFER, numVertices * sizeof(float) * 4 * 3, NULL, GL_STATIC_DRAW);
    glBufferSubData(GL_ARRAY_BUFFER, 0, numVertices * sizeof(float) * 4, vertex);
    glBufferSubData(GL_ARRAY_BUFFER, numVertices * sizeof(float) * 4, numVertices * sizeof(float) * 4, normal);
    glBufferSubData(GL_ARRAY_BUFFER, numVertices * sizeof(float) * 4 * 2, numVertices * sizeof(float) * 4, textco);

    glEnableVertexAttribArray(VERTEX_COORD_ATTRIB);
    glVertexAttribPointer(VERTEX_COORD_ATTRIB, 4, GL_FLOAT, 0, 0, 0);
    glEnableVertexAttribArray(NORMAL_ATTRIB);
    glVertexAttribPointer(NORMAL_ATTRIB, 4, GL_FLOAT, 0, 0, (void *) (numVertices * sizeof(float) * 4));
    glEnableVertexAttribArray(TEXTURE_COORD_ATTRIB);
    glVertexAttribPointer(TEXTURE_COORD_ATTRIB, 4, GL_FLOAT, 0, 0, (void *) (numVertices * sizeof(float) * 4 * 2));

    //index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[1]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh[objId].numIndexes, faceIndex, GL_STATIC_DRAW);

// unbind the VAO
    glBindVertexArray(0);

    mesh[objId].type = GL_TRIANGLES;
}

int revSmoothNormal2(float *p, float *nx, float *ny, float smoothCos, int beginEnd) {

    float v1x, v1y, v2x, v2y, x, y, norm;
    float auxX, auxY, angle;

    auxX = p[0] - p[2];
    auxY = p[1] - p[3];
    v1x = -auxY;
    v1y = auxX;
    norm = sqrt((v1x * v1x) + (v1y * v1y));
    v1x /= norm;
    v1y /= norm;

    auxX = p[2] - p[4];
    auxY = p[3] - p[5];
    v2x = -auxY;
    v2y = auxX;
    norm = sqrt((v2x * v2x) + (v2y * v2y));
    v2x /= norm;
    v2y /= norm;

    angle = v1x * v2x + v1y * v2y;

    if (angle > smoothCos) {
        x = v1x + v2x;
        y = v1y + v2y;
    } else if (beginEnd == 0) {
        x = v2x;
        y = v2y;
    } else {
        x = v1x;
        y = v1y;

    }

    norm = sqrt(x * x + y * y);

    assert(norm != 0);
    x /= norm;
    y /= norm;

    *nx = x;
    *ny = y;
    if (angle > smoothCos)
        return 1;
    else
        return 0;
}

float *circularProfile(float minAngle, float maxAngle, float radius, int divisions, float transX, float transY) {
    float *p = (float *) malloc(sizeof(float) * 2 * (divisions + 3));
    float step = (maxAngle - minAngle) / divisions;

    for (int i = 0, k = -1; i < divisions + 3; ++i, ++k) {

        p[i * 2] = radius * cos(minAngle + k * step) + transX;
        p[i * 2 + 1] = radius * sin(minAngle + k * step) + transY;
    }
    return p;
}
*/
