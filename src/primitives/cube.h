#include "../Vector3.h"

#define ONE 1.0f
int faceCount = 12;

float * get_shifted_vertices(Vector3 min, Vector3 max) {
    static float shifted_vertices[96] = {
            min.x, max.y, max.z, ONE,
            min.x, min.y, max.z, ONE,
            max.x, min.y, max.z, ONE,
            max.x, max.y, max.z, ONE,

            max.x, max.y, min.z, ONE,
            max.x, min.y, min.z, ONE,
            min.x, min.y, min.z, ONE,
            min.x, max.y, min.z, ONE,

            max.x, max.y, max.z, ONE,
            max.x, min.y, max.z, ONE,
            max.x, min.y, min.z, ONE,
            max.x, max.y, min.z, ONE,

            min.x, max.y, min.z, ONE,
            min.x, max.y, max.z, ONE,
            max.x, max.y, max.z, ONE,
            max.x, max.y, min.z, ONE,

            min.x, max.y, min.z, ONE,
            min.x, min.y, min.z, ONE,
            min.x, min.y, max.z, ONE,
            min.x, max.y, max.z, ONE,

            min.x, min.y, max.z, ONE,
            min.x, min.y, min.z, ONE,
            max.x, min.y, min.z, ONE,
            max.x, min.y, max.z, ONE
    };

    return shifted_vertices;
}

float vertices[] = {
	0.0f, 1.0f, 1.0f, ONE,
	0.0f, 0.0f, 1.0f, ONE,
	1.0f, 0.0f, 1.0f, ONE,
	1.0f, 1.0f, 1.0f, ONE,

	1.0f, 1.0f, 0.0f, ONE,
	1.0f, 0.0f, 0.0f, ONE,
	0.0f, 0.0f, 0.0f, ONE,
	0.0f, 1.0f, 0.0f, ONE,

	1.0f, 1.0f, 1.0f, ONE,
	1.0f, 0.0f, 1.0f, ONE,
	1.0f, 0.0f, 0.0f, ONE,
	1.0f, 1.0f, 0.0f, ONE,

	0.0f, 1.0f, 0.0f, ONE,
	0.0f, 1.0f, 1.0f, ONE,
	1.0f, 1.0f, 1.0f, ONE,
	1.0f, 1.0f, 0.0f, ONE,

	0.0f, 1.0f, 0.0f, ONE,
	0.0f, 0.0f, 0.0f, ONE,
	0.0f, 0.0f, 1.0f, ONE,
	0.0f, 1.0f, 1.0f, ONE,

	0.0f, 0.0f, 1.0f, ONE,
	0.0f, 0.0f, 0.0f, ONE,
	1.0f, 0.0f, 0.0f, ONE,
	1.0f, 0.0f, 1.0f, ONE
};

float centered_vertices[] = {
        -0.5f, +0.5f, +0.5f, ONE,
        -0.5f, -0.5f, +0.5f, ONE,
        +0.5f, -0.5f, +0.5f, ONE,
        +0.5f, +0.5f, +0.5f, ONE,

        +0.5f, +0.5f, -0.5f, ONE,
        +0.5f, -0.5f, -0.5f, ONE,
        -0.5f, -0.5f, -0.5f, ONE,
        -0.5f, +0.5f, -0.5f, ONE,

        +0.5f, +0.5f, +0.5f, ONE,
        +0.5f, -0.5f, +0.5f, ONE,
        +0.5f, -0.5f, -0.5f, ONE,
        +0.5f, +0.5f, -0.5f, ONE,

        -0.5f, +0.5f, -0.5f, ONE,
        -0.5f, +0.5f, +0.5f, ONE,
        +0.5f, +0.5f, +0.5f, ONE,
        +0.5f, +0.5f, -0.5f, ONE,

        -0.5f, +0.5f, -0.5f, ONE,
        -0.5f, -0.5f, -0.5f, ONE,
        -0.5f, -0.5f, +0.5f, ONE,
        -0.5f, +0.5f, +0.5f, ONE,

        -0.5f, -0.5f, +0.5f, ONE,
        -0.5f, -0.5f, -0.5f, ONE,
        +0.5f, -0.5f, -0.5f, ONE,
        +0.5f, -0.5f, +0.5f, ONE
};

float quad_vertices[] = {   //Quad 
	-0.5f, -0.5f, 0.0f, 1.0f,  //BL
	 0.5f, -0.5f, 0.0f, 1.0f,	//BR
	 0.5f,  0.5f, 0.0f, 1.0f,	//TR
	-0.5f,  0.5f, 0.0f, 1.0f,	//TL
};
    
float quad_vertices_0[] = {   //Quad 
	-3.0f, -3.0f, 0.0f, 1.0f,  //BL
	 3.0f, -3.0f, 0.0f, 1.0f,	//BR
	 3.0f,  3.0f, 0.0f, 1.0f,	//TR
	-3.0f,  3.0f, 0.0f, 1.0f,	//TL
};
float quad_normals[] = {   //Quad normals
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f
};

float normals[] = {   //cube normals
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
	0.0f, -1.0f, 0.0f, 0.0f
};

float texCoords[] = {
	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,

	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,

	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,

	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,

	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,

	0.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f
};


float quad_texCoords[] = {
	0.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 0.0f, 0.0f, 1.0f,
	1.0f, 1.0f, 0.0f, 1.0f,
	0.0f, 1.0f, 0.0f, 1.0f
};

unsigned int faceIndex[] = {
	0,1,2,0,2,3,
	4,5,6,4,6,7,
	8,9,10,8,10,11,
	12,13,14,12,14,15,
	16,17,18,16,18,19,
	20,21,22,20,22,23
};

unsigned int quad_faceIndex[] = {
	0,1,2,2,3,0
};