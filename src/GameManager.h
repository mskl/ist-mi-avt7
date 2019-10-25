//
// Created by skalimat on 11.10.19.
//

#ifndef AVT7_GAMEMANAGER_H
#define AVT7_GAMEMANAGER_H

#include <math.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include "stdio.h"

using namespace std;

#include <GL/glew.h>     // include GLEW to access OpenGL 3.3 functions
#include <GL/freeglut.h> // GLUT is the toolkit to interface with the OS

// Use Very Simple Libs
#include "libs/AVTmathLib.h"
#include "VertexAttrDef.h"
#include "libs/vsShaderLib.h"
#include "Camera.h"
#include "CameraPerspective.h"
#include "CameraPerspectiveMoving.h"
#include "CameraOrthogonal.h"
#include "objects/River.h"
#include "objects/Ground.h"
#include "objects/Player.h"
#include "objects/Road.h"
#include "objects/Light.h"
#include "objects/Bus.h"
#include "objects/Coordinates.h"
#include "objects/Log.h"
#include "objects/Sidewalls.h"
#include "objects/SceneCollider.h"
#include "objects/Target.h"
#include "objects/SpotLight.h"
#include "objects/DirectionalLight.h"
#include "objects/PointLight.h"
#include "objects/Car.h"
#include "objects/Turtle.h"

const char* VERTEX_SHADER_PATH = "shaders/phong.vert";
const char* FRAGMENT_SHADER_PATH = "shaders/phong.frag";

GLint deltaTime = 1;
GLint prevTime = 1;

GLint pvm_uniformId;
GLint vm_uniformId;
GLint normal_uniformId;

// Light GLSL stuff
GLint l_pos_id[8];  // pointers to shader variables
GLint l_enabled_id; // GLSL pointer to the boolean array
GLint l_enabled[8] = {1, 1, 1, 1, 1, 1, 1, 1};
GLint l_spot_dir_id;

GLint texMode_uniformId;
GLint tex_loc0, tex_loc1, tex_loc2, tex_loc3, tex_loc4;
GLuint TextureArray[25];


class GameManager {
public:
    GLint WindowHandle = 0;
    GLint FrameCount = 0;
    string infoString = "";

    bool isPlaying = true;

    int startingLives = 5;
    int currentLives = startingLives;
    int score = 0;
    int pointsPerTarget = 100;
    float speedMultiplier = 1.2f;

    const float topVerticalLimitPlayerPos = -6.0f;
    const float bottomVerticalLimitPlayerPos = 6.0f;
    const float leftHorizontalLimitPlayerPos = -6.0f;
    const float rightHorizontalLimitPlayerPos = 6.0f;

    vector<GameObject*> gameObjects = vector<GameObject*>();
    vector<Bus*> busses = vector<Bus*>();
    vector<Car*> cars = vector<Car*>();
    vector<Log*> logs = vector<Log*>();
    vector<Turtle*> turtles = vector<Turtle*>();

    // All in microseconds
    GLint lastMoveTime = 0;
    GLint moveTimeout = 300;


    enum CameraType {
        CAMERA_PERSPECTIVE_FOLLOW,
        CAMERA_PERSPECTIVE_FIXED,
        CAMERA_ORTHO
    };

    CameraType currentCameraType = CAMERA_PERSPECTIVE_FOLLOW;
    CameraPerspectiveMoving cameraPerspectiveMoving
            = CameraPerspectiveMoving(20, 0, 40, Vector3(0, 20, 0));
    CameraPerspective cameraPerspectiveFixed
            = CameraPerspective(20, 0, 90, Vector3(0, 20, 0));
    CameraOrthogonal cameraOrthogonal
            = CameraOrthogonal(-7, 8, -8, 7);

    // Player
    Player* player = new Player(Vector3(0, 1, 6));

    SceneCollider* sceneCollider = new SceneCollider(Vector3(-6.0f, -1, -6));
    Target* target = new Target(Vector3(0.25f, 1.25f, -5.75f));

    // Lights
    Light* directionalLight = new DirectionalLight(Vector3(0.0f, 5.0f, 0.0f), 6, false);
    SpotLight* spotLight = new SpotLight(Vector3(0, -1, 0), Vector3(0, 2, 0), 7, false);
    vector<PointLight*> pointLights = vector<PointLight*>();


    unsigned int lifes, points, gameover;
public:
    GameManager() {
        // Keep the pointLight in separate vector as well.
        pointLights.push_back(new PointLight(Vector3(-4.0f, 3.0f, -6.0f), 0, true));
        pointLights.push_back(new PointLight(Vector3(-4.0f, 3.0f, 0.0f), 1, true));
        pointLights.push_back(new PointLight(Vector3(-4.0f, 3.0f, 7.0f), 2, true));
        pointLights.push_back(new PointLight(Vector3(5.0f, 3.0f, -6.0f), 3, true));
        pointLights.push_back(new PointLight(Vector3(5.0f, 3.0f, 0.0f), 4, true));
        pointLights.push_back(new PointLight(Vector3(5.0f, 3.0f, 7.0f), 5, true));

        gameObjects.push_back(new River());
        gameObjects.push_back(new Road());
        gameObjects.push_back(new Ground());
        gameObjects.push_back(new Sidewalls());

        // Save the lights to gameObjects
        for (auto &pl : pointLights)
            gameObjects.push_back(pl);

        gameObjects.push_back(directionalLight);
        gameObjects.push_back(spotLight);

        gameObjects.push_back(player);
        gameObjects.push_back(sceneCollider);
        gameObjects.push_back(target);
    }

    void changeSize(int w, int h)
    {
        float ratio;
        // Prevent a divide by zero, when window is too short
        if (h == 0) h = 1;

        // set the viewport to be the entire window
        glViewport(0, 0, w, h);
        loadIdentity(PROJECTION);

        selectCamera(currentCameraType);
    }

    void processKeys(unsigned char key, int xx, int yy)
    {
        switch(key) {
            // Escape exits the game
            case 27: glutLeaveMainLoop(); break;
                // Quality settings
            case 'm': glEnable(GL_MULTISAMPLE); break;
            case 'M': glDisable(GL_MULTISAMPLE); break;
                // Player movement
            case 'p': movePlayer(1, 0); break;
            case 'o': movePlayer(-1, 0); break;
            case 'q': movePlayer(0, -1); break;
            case 'a': movePlayer(0, 1); break;
                // Respawn player
            case 'R': player->respawn(); break;
                // Random target position
            case 'v': target->setRandomPosition(); break;
                // Increase speed
            case 'i': increaseSpeed(); break;
                // Night lights
            case 'n': directionalLight->light_enabled = !directionalLight->light_enabled; break;
            case 'h': spotLight->light_enabled = !spotLight->light_enabled; break;
            case 'c':
                for (auto &c: pointLights)
                    c->light_enabled = !c->light_enabled;
                break;
            // Stop/Continue game
            case 's':
                if (currentLives != 0) {
                    isPlaying = !isPlaying;
                }
                break;
            case 'r':
                if (currentLives <= 0) {
                    respawnPlayer();
                    resetAllObjects();
                    currentLives = 5;
                    isPlaying = true;
                    infoString = "";
                }
                break;

            // CameraType
            case '1': selectCamera(CAMERA_PERSPECTIVE_FOLLOW); break;
            case '2': selectCamera(CAMERA_PERSPECTIVE_FIXED); break;
            case '3': selectCamera(CAMERA_ORTHO); break;
        }
    }

    void movePlayer(float x, float z) {
        GLint curTime = glutGet(GLUT_ELAPSED_TIME);

        if (curTime - lastMoveTime < moveTimeout)
            return;
        else if(!isPlaying)
            return;

        Vector3 moveVec = Vector3(x, 0, z);
        Vector3 tempPos = player->position + moveVec;
        if(tempPos.z < topVerticalLimitPlayerPos || tempPos.z > bottomVerticalLimitPlayerPos ||
           tempPos.x > rightHorizontalLimitPlayerPos || tempPos.x < leftHorizontalLimitPlayerPos)
            return;

        player->jump(moveVec, 0.5f);
        lastMoveTime = curTime;
    }

    GLuint setupShaders() {
        // Shader for models
        shader.init();
        shader.loadShader(VSShaderLib::VERTEX_SHADER, VERTEX_SHADER_PATH);
        shader.loadShader(VSShaderLib::FRAGMENT_SHADER, FRAGMENT_SHADER_PATH);

        // set semantics for the shader variables
        glBindFragDataLocation(shader.getProgramIndex(), 0,"colorOut");
        glBindAttribLocation(shader.getProgramIndex(), VERTEX_COORD_ATTRIB, "position");
        glBindAttribLocation(shader.getProgramIndex(), NORMAL_ATTRIB, "normal");
        glBindAttribLocation(shader.getProgramIndex(), TEXTURE_COORD_ATTRIB, "texCoord");
        glBindAttribLocation(shader.getProgramIndex(), VERTEX_ATTRIB1, "vVertex");
        glBindAttribLocation(shader.getProgramIndex(), VERTEX_ATTRIB2, "vtexCoord");
        glBindAttribLocation(shader.getProgramIndex(), TANGENT_ATTRIB, "tangent");

        glLinkProgram(shader.getProgramIndex());

        texMode_uniformId = glGetUniformLocation(shader.getProgramIndex(), "texMode");
        tex_loc0 = glGetUniformLocation(shader.getProgramIndex(), "texmap0");
        // Get the indexes of stuff
        pvm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_pvm");
        vm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_viewModel");
        normal_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_normal");

        // Get the light indexes
        for (int i = 0; i < 8; i++) {
            std::string pos = "l_pos_" + to_string(i);
            l_pos_id[i] = glGetUniformLocation(shader.getProgramIndex(), pos.c_str());
        }

        // Get the index of a light boolean mask
        l_enabled_id = glGetUniformLocation(shader.getProgramIndex(), "l_enabled");
        l_spot_dir_id = glGetUniformLocation(shader.getProgramIndex(), "l_spot_dir");

        printf("InfoLog for Per Fragment Phong Lightning Shader\n%s\n\n", shader.getAllInfoLogs().c_str());
        return(shader.isProgramLinked());
    }

    void initScene(){
        srand(time(NULL));

        createBus();
        createLogs();
        createCars();
        createTurtles();

        // Initialize all of the GameObjects
        for (GameObject* go : gameObjects) {
            go->init();
        }

        // some GL settings
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_MULTISAMPLE);
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        //initFonts();
    }

    void renderScene() {

        GLint currentTime = glutGet(GLUT_ELAPSED_TIME);
        deltaTime = prevTime - currentTime;
        prevTime = currentTime;

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        loadIdentity(VIEW);
        loadIdentity(MODEL);

        if (currentCameraType == CAMERA_PERSPECTIVE_FOLLOW) {
            cameraPerspectiveMoving.pos = player->position;
            cameraPerspectiveMoving.view();
        } else if (currentCameraType == CAMERA_PERSPECTIVE_FIXED) {
            cameraPerspectiveFixed.view();
        } else if (currentCameraType == CAMERA_ORTHO) {
            cameraOrthogonal.view();
        }

        // use our shader
        glUseProgram(shader.getProgramIndex());

        if (!shader.isProgramValid()) {
            printf("Program Not Valid!\n");
            exit(1);
        }

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureArray[0]);

        glUniform1i(tex_loc0, 0);

        // Update conelight position
        spotLight->position = player->position + Vector3(0.5f, 1.2f, 0.5f);
        spotLight->light_dir = Vector3(0, 0, -1);

        bool riverBorder = false;
        bool roadDeath = false;
        bool hitRiver = false;
        bool hitLog = false;

        for (GameObject *go : gameObjects) {
            if (go->isEnabled()) {

                // Update the physics
                if (isPlaying){
                    go->update(deltaTime);

                    if(go->getType() == TARGET){
                        // TODO: put this into an update method of the Target object, there is no need to have it here
                        target->rotateCube(deltaTime);
                    }


                    if(go->getType() == BUS){
                        ((Bus*)go)->rotateWheels();
                    }
                    if(go->getType() == CAR){
                        ((Car*)go)->rotateWheels();
                    }
                    if(go->getType() == LOG){
                        ((Log*)go)->rockLog();
                    }
                }

                // Check the collisions
                checkBusCollisions(go);
                checkCarCollisions(go);
                checkLogCollisions(go);
                checkTurtlesCollisions(go);

                // Render the objects
                go->render();

                // Check collisions with player
                if (!(go->position == player->position)) {
                    if (player->collideWith(go)) {
                        if (go->getType() == BUS || go->getType() == CAR) {
                            roadDeath = true;
                        } else if(go->getType() == TARGET){
                            target->setRandomPosition();
                            increaseSpeed();
                            player->respawn();
                            score += pointsPerTarget;
                        }
                    } else if ((player->playerState == GROUNDED) && (player->collideWithBottom(go))) {
                        cout << "Bottom collision with " << go->getType() << endl;
                        if (go->getType() == LOG) {
                            hitLog = true;
                            player->playerState = ONLOG;
                            player->speed = go->getSpeed();
                        } else if (go->getType() == TURTLE) {
                            Turtle* turt = (Turtle*)go;
                            cout << turt->isUnderWater << endl;
                            if(turt->isUnderWater){
                                hitRiver = true;
                            }else{
                                hitLog = true;
                                player->playerState = ONTURTLE;
                                player->speed = go->getSpeed();
                            }
                        } else if (go->getType() == RIVER) {
                            hitRiver = true;
                        }
                    }else if (player->playerState == ONTURTLE){
                        if (go->getType() == TURTLE) {
                            Turtle* turt = (Turtle*) go;
                            if(turt->isUnderWater){
                                hitRiver = true;
                            }
                        }

                        if(go->getType() == BOUNDS && player->playerState == ONTURTLE && !player->collideWith(go)){
                            riverBorder = true;
                        }
                    }else{
                        if(go->getType() == BOUNDS && player->playerState == ONLOG){
                            riverBorder = true;
                        }

                    }
                }
            }
        }

        bool deathInRiver = hitRiver && (!hitLog) && (player->playerState != ONLOG);

        if (riverBorder) {
            cout << "Death in river border!" << endl;
            onDeath();
        }
        if (deathInRiver) {
            cout << "Death in river!" << endl;
            onDeath();
        }
        if (roadDeath) {
            cout << "Death on the road!" << endl;
            onDeath();
        }

        /*if(!isPlaying){
            vsfl.renderSentence(10, 50, gameover);
        }*/

        glutSwapBuffers();
    }

private:
    void onDeath(){
        currentLives--;

        if (currentLives == 0) {
            infoString = "Player has died! The achieved score: " + to_string(score);
            isPlaying = false;
        }

        respawnPlayer();
    }

    void respawnPlayer() {
        player->playerState = GROUNDED;
        player->speed = Vector3(0, 0, 0);
        player->respawn();
    }
    void resetAllObjects() {
        std::vector<Bus *>::iterator bus_obj;
        for (bus_obj = busses.begin(); bus_obj != busses.end(); bus_obj++) {
            (*bus_obj)->speed = (*bus_obj)->initSpeed;
        }
        std::vector<Log *>::iterator log_obj;
        for (log_obj = logs.begin(); log_obj != logs.end(); log_obj++) {
            (*log_obj)->speed = (*log_obj)->initSpeed;
        }

        std::vector<Car *>::iterator car_obj;
        for (car_obj = cars.begin(); car_obj != cars.end(); car_obj++) {
            (*car_obj)->speed = (*car_obj)->initSpeed;
        }
        std::vector<Turtle *>::iterator turtle_obj;
        for (turtle_obj = turtles.begin(); turtle_obj != turtles.end(); turtle_obj++) {
            (*turtle_obj)->speed = (*turtle_obj)->initSpeed;
        }
    }


    void increaseSpeed() {
        std::vector<Bus *>::iterator bus_obj;
        for (bus_obj = busses.begin(); bus_obj != busses.end(); bus_obj++) {
            (*bus_obj)->speed = (*bus_obj)->speed*speedMultiplier;
        }
        std::vector<Log *>::iterator log_obj;
        for (log_obj = logs.begin(); log_obj != logs.end(); log_obj++) {
            (*log_obj)->speed = (*log_obj)->speed*speedMultiplier;
        }

        std::vector<Car *>::iterator car_obj;
        for (car_obj = cars.begin(); car_obj != cars.end(); car_obj++) {
            (*car_obj)->speed = (*car_obj)->speed*speedMultiplier;
        }
        std::vector<Turtle *>::iterator turtle_obj;
        for (turtle_obj = turtles.begin(); turtle_obj != turtles.end(); turtle_obj++) {
            (*turtle_obj)->speed = (*turtle_obj)->speed*speedMultiplier;
        }
    }

    void selectCamera(CameraType newCamera) {
        currentCameraType = newCamera;

        loadIdentity(PROJECTION);
        if (newCamera == CAMERA_PERSPECTIVE_FOLLOW) {
            cameraPerspectiveMoving.project(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        } else if (newCamera == CAMERA_PERSPECTIVE_FIXED) {
            cameraPerspectiveFixed.project(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        } else if (newCamera == CAMERA_ORTHO) {
            cameraOrthogonal.project(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        }
    }

    void createBus(){
        Bus * bus;
        for (int i = 0; i < 1; i++){
            float randSpeed =(float)(rand() % 80 + 50) / 100.0f;
            for (int j = 0; j < 4; j++){
                int offset = rand() % 7 + 1;
                bool isGoingRight = i == 1;
                if(isGoingRight){
                    Vector3 spawnPosition = Vector3(-7.0f - offset, 1, i*2+1);
                    if(j > 0){
                        spawnPosition.x = busses.back()->position.x - 5.0f * i - offset;
                    }
                    //bus = new Bus(spawnPosition, Vector3(randSpeed*-1, 0, 0), false);
                    bus = new Bus(spawnPosition, Vector3(-1, 0, 0), true);
                }else{
                    Vector3 spawnPosition = Vector3(7.0f + offset, 1, i*2+1);
                    if(j > 0){
                        spawnPosition.x = busses.back()->position.x + 5.0f * i + offset;
                    }
                    bus = new Bus(spawnPosition, Vector3(randSpeed, 0, 0), false);
                }
                gameObjects.push_back(bus);
                busses.push_back(bus);
            }
        }
    }

    void createCars(){
        Car * car;
        for (int i = 0; i < 2; i++){
            float randSpeed =(float)(rand() % 80 + 50) / 100.0f;
            for (int j = 0; j < 5; j++){
                int offset = rand() % 4 + 1;
                bool isGoingRight = i == 1;
                if(isGoingRight){
                    Vector3 spawnPosition = Vector3(-7.0f - offset, 1, 2+i*2+1);
                    if(j > 0){
                        spawnPosition.x = cars.back()->position.x - 2.0f * i - offset;
                    }
                    cout << spawnPosition.x << endl;
                    car = new Car(spawnPosition, Vector3(-1, 0, 0), true);
                }else{
                    Vector3 spawnPosition = Vector3(7.0f + offset, 1, 2+i*2+1);
                    if(j > 0){
                        spawnPosition.x = cars.back()->position.x + 5.0f * i + offset;
                    }
                    car = new Car(spawnPosition, Vector3(randSpeed, 0, 0), false);
                }
                gameObjects.push_back(car);
                cars.push_back(car);
            }
        }
    }

    void checkBusCollisions(GameObject* go){
        std::vector<Bus *>::iterator it_obj;
        for (it_obj = busses.begin(); it_obj != busses.end(); it_obj++) {
            if (!(go->position == (*it_obj)->position)) {
                if (!(*it_obj)->collideWith(go)) {
                    if (go->getType() == BOUNDS) {
                        if ((*it_obj)->position.x < go->position.x && !(*it_obj)->isGoingRight) {
                            (*it_obj)->respawn();
                        }
                        if ((*it_obj)->position.x > go->position.x && (*it_obj)->isGoingRight) {
                            (*it_obj)->respawn();
                        }
                    }
                }
                if ((*it_obj)->collideWith(go)) {
                    if (go->getType() == BUS) {
                        GameObject *objA = ((*it_obj)->position.x > go->position.x) ? (*it_obj) : go;

                        Vector3 tempPos = objA->position;
                        if((*it_obj)->isGoingRight){
                            tempPos.x -= (float) (rand() % 5 + 3);
                        }else{
                            tempPos.x += (float) (rand() % 5 + 3);
                        }
                        objA->position = tempPos;
                    }
                }
            }
        }
    }

    void checkCarCollisions(GameObject* go){
        std::vector<Car *>::iterator it_obj;
        for (it_obj = cars.begin(); it_obj != cars.end(); it_obj++) {
            if (!(go->position == (*it_obj)->position)) {
                if (!(*it_obj)->collideWith(go)) {
                    if (go->getType() == BOUNDS) {
                        if ((*it_obj)->position.x < go->position.x && !(*it_obj)->isGoingRight) {
                            (*it_obj)->respawn();
                        }
                        if ((*it_obj)->position.x > go->position.x && (*it_obj)->isGoingRight) {
                            (*it_obj)->respawn();
                        }
                    }
                }
                if ((*it_obj)->collideWith(go)) {
                    if (go->getType() == CAR) {
                        GameObject *objA = ((*it_obj)->position.x > go->position.x) ? (*it_obj) : go;

                        Vector3 tempPos = objA->position;
                        if((*it_obj)->isGoingRight){
                            tempPos.x -= (float) (rand() % 5 + 3);
                        }else{
                            tempPos.x += (float) (rand() % 5 + 3);
                        }
                        objA->position = tempPos;
                    }
                }
            }
        }
    }

    void createLogs() {
        Log * log;
        for (int i = 0; i < 3; i++){
            float randSpeed =(float)(rand() % 60 + 20) / 100.0f;

            for (int j = 0; j < 4; j++){
                int offset = rand() % 7;

                Vector3 spawnPosition = Vector3(7.0f+j*3+offset, 0, -i*2-1);
                if (j > 0) {
                    spawnPosition.x = logs.back()->position.x+3.5f+offset;
                }
                log = new Log(spawnPosition, Vector3(randSpeed, 0, 0));
                gameObjects.push_back(log);
                logs.push_back(log);
            }
        }
    }

    void createTurtles() {
        Turtle * turtle;
        for (int i = 0; i < 2; i++){
            float randSpeed =(float)(rand() % 30 + 20) / 100.0f;

            for (int j = 0; j < 2; j++){
                int offset = rand() % 7;

                Vector3 spawnPosition = Vector3(-9.0f-j*3-offset, 0, -i*2-2);
                if(j > 0){
                    spawnPosition.x = turtles.back()->position.x-5.0f-offset;
                }
                turtle = new Turtle(spawnPosition, Vector3(randSpeed*-1, 0, 0));
                gameObjects.push_back(turtle);
                turtles.push_back(turtle);
            }
        }
    }

    void checkLogCollisions(GameObject* go){
        std::vector<Log *>::iterator it_obj;
        for (it_obj = logs.begin(); it_obj != logs.end(); it_obj++) {
            if (!(go->position == (*it_obj)->position)) {
                if (!(*it_obj)->collideWith(go)) {
                    if (go->getType() == BOUNDS) {
                        if ((*it_obj)->position.x < go->position.x) {
                            (*it_obj)->respawn();
                        }
                    }
                }
                if ((*it_obj)->collideWith(go)) {
                    if (go->getType() == LOG) {
                        GameObject *objA = ((*it_obj)->position.x > go->position.x) ? (*it_obj) : go;

                        Vector3 tempPos = objA->position;
                        tempPos.x += (float) (rand() % 5 + 3);
                        objA->position = tempPos;
                    }
                }
            }
        }
    }

    void checkTurtlesCollisions(GameObject* go){
        std::vector<Turtle *>::iterator it_obj;
        for (it_obj = turtles.begin(); it_obj != turtles.end(); it_obj++) {
            if (!(go->position == (*it_obj)->position)) {
                if (!(*it_obj)->collideWith(go)) {
                    if (go->getType() == BOUNDS) {
                        if ((*it_obj)->position.x > go->position.x) {
                            (*it_obj)->respawn();
                        }
                    }
                }
                if ((*it_obj)->collideWith(go)) {
                    if (go->getType() == TURTLE) {
                        GameObject *objA = ((*it_obj)->position.x < go->position.x) ? (*it_obj) : go;

                        Vector3 tempPos = objA->position;
                        tempPos.x -= (float) (rand() % 5 + 3);
                        objA->position = tempPos;
                    }
                }
            }
        }
    }
};

#endif //AVT7_GAMEMANAGER_H