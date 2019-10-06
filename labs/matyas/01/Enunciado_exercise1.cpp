#include <iostream>
#include <sstream>
#include <string>
#include <fstream>

#include <GL/glew.h>
#include <GL/freeglut.h>

#include "cube.h"

#define CAPTION "Exercise 1"

int WinX = 640, WinY = 480;
int WindowHandle = 0;
unsigned int FrameCount = 0;

#define VERTEX_COORD_ATTRIB 0
#define NORMAL_ATTRIB 1
#define TEXTURE_COORD_ATTRIB 2

GLuint VaoId, VboId[4];
GLuint VertexShaderId, FragmentShaderId, ProgramId;
GLint UniformId;

using namespace std;

/*
- Verify what OpenGL contexts your computer can support, focusing on OpenGL 2.1, OpenGL 3.2, OpenGL 3.3.
- Change the program so display is called at 60 FPS.
- Add functionality to your code in order to read shader code from external files;
- Check shader compilation and linkage for error messages.
- Draw a cube by using the information provided in file cube.h. Try to understand why the number of vertices are 24 instead of 8.
- Modify the function createBufferObjects(), by using both OGL functions: glBufferData() and glBufferSubData()
- Set up a perspective projection in the reshape() function and make it to support correctly the resizing of the window by the user
 */

/////////////////////////////////////////////////////////////////////// ERRORS

bool isOpenGLError() {
    bool isError = false;
    GLenum errCode;
    const GLubyte *errString;
    while ((errCode = glGetError()) != GL_NO_ERROR) {
        isError = true;
        errString = gluErrorString(errCode);
        std::cerr << "OpenGL ERROR [" << errString << "]." << std::endl;
    }
    return isError;
}

void checkOpenGLError(string error) {
    if (isOpenGLError()) {
        std::cerr << error << std::endl;
        exit(EXIT_FAILURE);
    }
}

/////////////////////////////////////////////////////////////////////// SHADERs

const GLchar* VertexShader =
        {
                "#version 330 core\n"

                "in vec4 in_Position;\n"
                "uniform mat4 Matrix;\n"
                "out vec4 color;\n"

                "void main(void)\n"
                "{\n"
                "	color = in_Position;\n"
                "	gl_Position = Matrix * in_Position;\n"

                "}\n"
        };

const GLchar* FragmentShader =
        {
                "#version 330 core\n"

                "in vec4 color;\n"
                "out vec4 out_Color;\n"

                "void main(void)\n"
                "{\n"
                "	out_Color = color;\n"
                "}\n"
        };

void read_shader_file(const char *shader_file, string & str) {
    std::ifstream ifs(shader_file);
    str = string((std::istreambuf_iterator<char>(ifs)),(std::istreambuf_iterator<char>()));
}

void createShaderProgram() {
    VertexShaderId = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShaderId, 1, &VertexShader, 0);
    glCompileShader(VertexShaderId);

    FragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShaderId, 1, &FragmentShader, 0);
    glCompileShader(FragmentShaderId);

    ProgramId = glCreateProgram();
    glAttachShader(ProgramId, VertexShaderId);
    glAttachShader(ProgramId, FragmentShaderId);

    glBindAttribLocation(ProgramId, VERTEX_COORD_ATTRIB, "in_Position");

    glLinkProgram(ProgramId);
    UniformId = glGetUniformLocation(ProgramId, "Matrix");

    checkOpenGLError("ERROR: Could not create shaders.");
}

void destroyShaderProgram() {
    glUseProgram(0);
    glDetachShader(ProgramId, VertexShaderId);
    glDetachShader(ProgramId, FragmentShaderId);

    glDeleteShader(FragmentShaderId);
    glDeleteShader(VertexShaderId);
    glDeleteProgram(ProgramId);

    checkOpenGLError("ERROR: Could not destroy shaders.");
}

/////////////////////////////////////////////////////////////////////// VAOs & VBOs


void createBufferObjects() {
    glGenVertexArrays(1, &VaoId);
    glBindVertexArray(VaoId);

    glGenBuffers(4, VboId);

    // vertex coordinates buffer
    glBindBuffer(GL_ARRAY_BUFFER, VboId[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glEnableVertexAttribArray(VERTEX_COORD_ATTRIB);
    glVertexAttribPointer(VERTEX_COORD_ATTRIB, 4, GL_FLOAT, 0, 0, 0);

    // normals buffer
    glBindBuffer(GL_ARRAY_BUFFER, VboId[2]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(normals), normals, GL_STATIC_DRAW);
    glEnableVertexAttribArray(NORMAL_ATTRIB);
    glVertexAttribPointer(NORMAL_ATTRIB, 3, GL_FLOAT, 0, 0, 0);

    // texture coordinates buffer
    glBindBuffer(GL_ARRAY_BUFFER, VboId[1]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(texCoords), texCoords, GL_STATIC_DRAW);
    glEnableVertexAttribArray(TEXTURE_COORD_ATTRIB);
    glVertexAttribPointer(TEXTURE_COORD_ATTRIB, 2, GL_FLOAT, 0, 0, 0);

    // index buffer
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, VboId[3]);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(faceIndex), faceIndex, GL_STATIC_DRAW);

    // unbind the VAO
    glBindVertexArray(0);

    checkOpenGLError("ERROR: Could not create VAOs and VBOs.");
}

void destroyBufferObjects() {
    glDeleteBuffers(4, VboId);
    glDeleteVertexArrays(1, &VaoId);
    checkOpenGLError("ERROR: Could not destroy VAOs and VBOs.");
}

/////////////////////////////////////////////////////////////////////// SCENE

typedef GLfloat Matrix[16];

const Matrix I = {
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
}; // Row Major (GLSL is Column Major)

const Matrix M = {
        1.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, -1.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
}; // Row Major (GLSL is Column Major)

const Matrix C = {
        1.0f, 0.0f, 0.0f, -0.5f,
        0.0f, 1.0f, 0.0f, -0.5f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
}; // Row Major (GLSL is Column Major)

void renderScene() {
    glBindVertexArray(VaoId);
    glUseProgram(ProgramId);

    glUniformMatrix4fv(UniformId, 1, GL_TRUE, C);
    glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_INT, (GLvoid *) 0);

    //glUniformMatrix4fv(UniformId, 1, GL_TRUE, M);
    //glDrawElements(GL_TRIANGLES, faceCount * 3, GL_UNSIGNED_INT, (GLvoid *) 0);

    glUseProgram(0);
    glBindVertexArray(0);

    checkOpenGLError("ERROR: Could not draw scene.");
}

/////////////////////////////////////////////////////////////////////// CALLBACKS

void cleanup() {
    destroyShaderProgram();
    destroyBufferObjects();
}

// The last time the display was drawn
int previous_draw_time = 0;

void display() {
    ++FrameCount;
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    renderScene();
    glutSwapBuffers();

    previous_draw_time = glutGet(GLUT_ELAPSED_TIME);
}

void reshape(int w, int h) {
    WinX = w;
    WinY = h;
    glViewport(0, 0, WinX, WinY);
}

void timer(int value) {
    std::ostringstream oss;
    oss << CAPTION << ": " << FrameCount << " FPS @ (" << WinX << "x" << WinY << ")";
    std::string s = oss.str();
    glutSetWindow(WindowHandle);
    glutSetWindowTitle(s.c_str());
    FrameCount = 0;
    glutTimerFunc(1000, timer, 0);
}

void refresh(int value)
{
    glutPostRedisplay();
    glutTimerFunc(1000/60, refresh, 0);
}

/////////////////////////////////////////////////////////////////////// SETUP

void setupCallbacks() {
    glutCloseFunc(cleanup);
    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutTimerFunc(0, timer, 0);
    glutTimerFunc(0, refresh, 0);
}

void setupOpenGL() {
    std::cerr << "CONTEXT: OpenGL v" << glGetString(GL_VERSION) << std::endl;
    glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glDepthMask(GL_TRUE);
    glDepthRange(0.0, 1.0);
    glClearDepth(1.0);
    glEnable(GL_CULL_FACE);
    glCullFace(GL_BACK);
    glFrontFace(GL_CCW);
}

void setupGLEW() {
    glewExperimental = GL_TRUE;
    GLenum result = glewInit();
    if (result != GLEW_OK) {
        std::cerr << "ERROR glewInit: " << glewGetString(result) << std::endl;
        exit(EXIT_FAILURE);
    }
    GLenum err_code = glGetError();
    printf("Vendor: %s\n", glGetString(GL_VENDOR));
    printf("Renderer: %s\n", glGetString(GL_RENDERER));
    printf("Version: %s\n", glGetString(GL_VERSION));
    printf("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

}

void setupGLUT(int argc, char *argv[]) {
    glutInit(&argc, argv);

    glutInitContextVersion(3, 3);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);

    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);

    glutInitWindowSize(WinX, WinY);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
    WindowHandle = glutCreateWindow(CAPTION);
    if (WindowHandle < 1) {
        std::cerr << "ERROR: Could not create a new rendering window." << std::endl;
        exit(EXIT_FAILURE);
    }
}

void init(int argc, char *argv[]) {
    setupGLUT(argc, argv);
    setupGLEW();
    setupOpenGL();
    createShaderProgram();
    createBufferObjects();
    setupCallbacks();
}

int main(int argc, char *argv[]) {
    init(argc, argv);
    glutMainLoop();
    exit(EXIT_SUCCESS);
}

///////////////////////////////////////////////////////////////////////