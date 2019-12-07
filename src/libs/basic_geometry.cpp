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


void createQuad(GLint _objId, float size_x, float size_y)
{
	int i;
	float vert[16];
	mesh[_objId].numIndexes = 2*3;

	memcpy(vert, quad_vertices, sizeof(float) * 16);

	for(i=0; i< 4; i++) {
		vert[i*4] *= size_x;
		vert[i*4+1] *= size_y;
	}

	glGenVertexArrays(1, &(mesh[_objId].vao));
	glBindVertexArray(mesh[_objId].vao);

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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * mesh[_objId].numIndexes, quad_faceIndex , GL_STATIC_DRAW);
    
    // unbind the VAO
    glBindVertexArray(0);

	mesh[_objId].type = GL_TRIANGLES;
}

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

void createCube(GLint _objId, bool centered) {
    mesh[_objId].numIndexes = faceCount * 3;

    glGenVertexArrays(1, &(mesh[_objId].vao));
    glBindVertexArray(mesh[_objId].vao);

    glGenBuffers(2, VboId);
    glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices) + sizeof(normals) + sizeof(texCoords), NULL, GL_STATIC_DRAW);
    if(centered) {
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), centered_vertices);
    } else {
        glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    }
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
