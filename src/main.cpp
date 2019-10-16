#include <math.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include "stdio.h"

using namespace std;

#include <GL/glew.h>     // include GLEW to access OpenGL 3.3 functions
#include <GL/freeglut.h> // GLUT is the toolkit to interface with the OS


#include "GameManager.h"

struct GameManagerWrapper {
    static void changeSize(int w, int h) {
        manager->changeSize(w, h);
    }

    static void renderScene() {
        manager->renderScene();
    }

    static void processKeys(unsigned char key, int xx, int yy) {
        manager->processKeys(key, xx, yy);
    }

    static void processMouseButtons(int button, int state, int xx, int yy) {
        manager->processMouseButtons(button, state, xx, yy);
    }

    static void processMouseMotion(int xx, int yy) {
        manager->processMouseMotion(xx, yy);
    }

    static void mouseWheel(int wheel, int direction, int x, int y) {
        manager->mouseWheel(wheel, direction, x, y);
    }

    static GameManager *manager;
};

GameManager* GameManagerWrapper::manager = nullptr;

void timer(int value)
{
	std::ostringstream oss;
	oss << CAPTION << ": " << GameManagerWrapper::manager->FrameCount <<
	    " FPS @ (" << GameManagerWrapper::manager->WinX << "x" << GameManagerWrapper::manager->WinY << ")";
	glutSetWindow(GameManagerWrapper::manager->WindowHandle);
    glutSetWindowTitle(oss.str().c_str());
    GameManagerWrapper::manager->FrameCount = 0;
    glutTimerFunc(1000, timer, 0);
}

void refresh(int value)
{
	glutPostRedisplay();
	glutTimerFunc(1000/60, refresh, 0);
}

void initialiseGlut(int argc, char **argv) {
    // GLUT initialization
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA | GLUT_MULTISAMPLE);

    glutInitContextVersion (3, 3);
    glutInitContextProfile (GLUT_CORE_PROFILE );
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE | GLUT_DEBUG);

    glutInitWindowPosition(100,100);
    glutInitWindowSize(GameManagerWrapper::manager->WinX, GameManagerWrapper::manager->WinY);
    GameManagerWrapper::manager->WindowHandle = glutCreateWindow(CAPTION);

    // Callback Registration
    glutDisplayFunc(GameManagerWrapper::renderScene);
    glutReshapeFunc(GameManagerWrapper::changeSize);

    glutTimerFunc(0, timer, 0);
    glutTimerFunc(0, refresh, 0);

    // Mouse and Keyboard Callbacks
    glutKeyboardFunc(GameManagerWrapper::processKeys);
    glutMouseFunc(GameManagerWrapper::processMouseButtons);
    glutMotionFunc(GameManagerWrapper::processMouseMotion);
    glutMouseWheelFunc(GameManagerWrapper::mouseWheel) ;

    // return from main loop
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE, GLUT_ACTION_GLUTMAINLOOP_RETURNS);
}

void initialiseGlew() {
    glewExperimental = GL_TRUE;
    glewInit();
}

int main(int argc, char **argv)
{
    GameManagerWrapper::manager = new GameManager();

    initialiseGlut(argc, argv);
    initialiseGlew();

	printf ("Version: %s\n", glGetString (GL_VERSION));
	if (!GameManagerWrapper::manager->setupShaders()) return(1);

    GameManagerWrapper::manager->initScene();
	glutMainLoop();

	return(0);
}