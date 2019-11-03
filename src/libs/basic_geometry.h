#include "../Vector3.h"

#define MAXIMUM_TEXTURES 4

struct Material{
	float diffuse[4];
	float ambient[4];
	float specular[4];
	float emissive[4];
	float shininess;
	int texcount;
};

// A model can be made of many meshes. Each is stored  in the following structure
struct MyMesh {
		GLuint vao;
		GLuint texUnits[MAXIMUM_TEXTURES];
		GLuint texTypes[MAXIMUM_TEXTURES];
		float transform[16];
		int numIndexes;
		unsigned int type;
		struct Material mat;
	};

void createCube(GLint id);
void createCubeMinMax(GLint id, Vector3 min, Vector3 max);
void createSphere(float radius, int divisions);
void createRectangle(float length, float height, float width, GLint mid);
//void computeVAO(int numP, float *p, float *pfloatoints, int sides, float smoothCos);
int revSmoothNormal2(float *p, float *nx, float *ny, float smoothCos, int beginEnd);
float *circularProfile(float minAngle, float maxAngle, float radius, int divisions, float transX= 0.0f, float transY = 0.0f);
