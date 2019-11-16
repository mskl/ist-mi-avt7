//
// Created by skalimat on 11.10.19.
//

#ifndef AVT7_GAMEMANAGER_H
#define AVT7_GAMEMANAGER_H

#include <math.h>
#include <iostream>
#include <sstream>
#include <string.h>
#include <stdio.h>
#include <iostream>
#include <GL/glew.h>     // include GLEW to access OpenGL 3.3 functions
#include <GL/freeglut.h> // GLUT is the toolkit to interface with the OS

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
#include "objects/SceneStencil.h"
#include "objects/SideCollider.h"
#include "libs/TGA.h"
#include "objects/Tree.h"
#include "objects/ParticleSystem.h"

// Macro to print filename shen using cout
#define mycout std::cout <<  __FILE__  << "(" << __LINE__ << ") "
#define cout mycout

using namespace std;

const char* VERTEX_SHADER_PATH = "shaders/phong.vert";
const char* FRAGMENT_SHADER_PATH = "shaders/phong.frag";

GLint deltaTime = 1;
GLint prevTime = 1;

GLint pvm_uniformId;
GLint vm_uniformId;
GLint normal_uniformId;

// Fog enabled is int used as a boolean
GLint fog_enabled_id;
GLint fog_enabled = 0;

// Light GLSL stuff
GLint l_pos_id[8];  // pointers to shader variables
GLint l_enabled_id; // GLSL pointer to the boolean array
GLint l_enabled[8] = {1, 1, 1, 1, 1, 1, 1, 1};
GLint l_spot_dir_id;

// Textures
GLint tex_road_loc, tex_river_loc, tex_grass_loc, tex_text_loc, tex_tree_loc, tex_particle_loc;
GLint texMode_uniformId;
GLuint TextureArray[6];

// Hud text
float _fontSize = 16;
GLuint text_vaoID;
GLuint text_texCoordBuffer;
GLuint text_vertexBuffer;
GLint doingText_uniformId;
GLint doingTextV_uniformId;

class GameManager {
public:
    GLint WindowHandle = 0;
    GLint FrameCount = 0;
    string infoString = "";

    bool isPlaying = true;

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
    vector<Tree*> trees = vector<Tree*>();

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

    // Particle system
    ParticleSystem* particleSystem = new ParticleSystem(Vector3(0, 3, 0));

    // Player
    Player* player = new Player(Vector3(0, 1, 6), particleSystem);

    Target* target = new Target(Vector3(0.25f, 1.25f, -5.75f));

    // The stencil cube
    SceneStencil* stencil = new SceneStencil();

    // Lights
    Light* directionalLight = new DirectionalLight(Vector3(0.0f, 5.0f, 0.0f), 6, false);
    SpotLight* spotLight = new SpotLight(Vector3(0, -1, 0), Vector3(0, 2, 0), 7, false);

    vector<PointLight*> pointLights = vector<PointLight*>();

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
        gameObjects.push_back(new Ground(Vector3(-13 / 2, 0, 6), Vector3(13, 1, 1)));
        gameObjects.push_back(new Ground(Vector3(-13 / 2, 0, 0), Vector3(13, 1, 1)));
        gameObjects.push_back(new Ground(Vector3(-6, 0, -6), Vector3(1, 1, 1)));
        gameObjects.push_back(new Ground(Vector3(-4, 0, -6), Vector3(1, 1, 1)));
        gameObjects.push_back(new Ground(Vector3(-2, 0, -6), Vector3(1, 1, 1)));
        gameObjects.push_back(new Ground(Vector3(0, 0, -6), Vector3(1, 1, 1)));
        gameObjects.push_back(new Ground(Vector3(2, 0, -6), Vector3(1, 1, 1)));
        gameObjects.push_back(new Ground(Vector3(4, 0, -6), Vector3(1, 1, 1)));
        gameObjects.push_back(new Ground(Vector3(6, 0, -6), Vector3(1, 1, 1)));

        trees.push_back(new Tree(Vector3(7.5, 2, -6)));
        trees.push_back(new Tree(Vector3(9, 2, -3)));
        trees.push_back(new Tree(Vector3(7.5, 2, 0)));
        trees.push_back(new Tree(Vector3(8.1, 2, 1)));
        trees.push_back(new Tree(Vector3(8, 2, 5)));

        trees.push_back(new Tree(Vector3(-6.9, 2, -5.5)));
        trees.push_back(new Tree(Vector3(-9.1, 2, -4)));
        trees.push_back(new Tree(Vector3(-8, 2, -2)));
        trees.push_back(new Tree(Vector3(-7, 2, 2)));
        trees.push_back(new Tree(Vector3(-9, 2, 4)));
        trees.push_back(new Tree(Vector3(-7.5, 2, 6)));

        // First deadly right, then deadly left
        gameObjects.push_back(new SideCollider(Vector3(7, -3, -5), Vector3(8, 3, 0), DEADLYBOUNDS));
        gameObjects.push_back(new SideCollider(Vector3(-7, -3, -5), Vector3(-6, 3, 0), DEADLYBOUNDS));

        gameObjects.push_back(new Sidewalls());
        gameObjects.push_back(particleSystem);

        // Save the lights to gameObjects
        for (auto &pl : pointLights) {
            gameObjects.push_back(pl);
        }

        gameObjects.push_back(directionalLight);
        gameObjects.push_back(spotLight);

        gameObjects.push_back(player);
        gameObjects.push_back(target);

        //if (USE_STENCIL) {
            gameObjects.push_back(stencil);
        //}
    }

    void changeSize(int w, int h) {
        // Prevent a divide by zero, when window is too short
        if (h == 0) h = 1;

        // set the viewport to be the entire window
        glViewport(0, 0, w, h);
        selectCamera(currentCameraType);
    }

    void processKeys(unsigned char key, int xx, int yy) {
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
            case 'i':
                increaseSpeedMultipliers(); break;
            // Night lights
            case 'n': directionalLight->toggleLight(); break;
            case 'h': spotLight->toggleLight(); break;
            case 'c':
                for (auto &c: pointLights)
                    c->toggleLight();
                break;
            // Stop/Continue game
            case 's':
                if (player->currentLives != 0) {
                    isPlaying = !isPlaying;
                    changeAnimationState(isPlaying);
                }
                break;
            // Restart the game if the player died
            case 'r':
                if (player->currentLives <= 0) {
                    resetSpeedMultipliers();
                    player->restartGame();
                    isPlaying = true;
                    infoString = "";
                }
                break;
            // Fog switch
            case 'f':
                fog_enabled = !fog_enabled;
                break;
            // Fireworks
            case 'e':
                particleSystem->spawnParticles(Vector3(0, 3, 0));
                break;
            // CameraType
            case '1': selectCamera(CAMERA_PERSPECTIVE_FOLLOW); break;
            case '2': selectCamera(CAMERA_PERSPECTIVE_FIXED); break;
            case '3': selectCamera(CAMERA_ORTHO); break;
        }
    }

    void movePlayer(float x, float z) {
        // Check the player movement timeout
        GLint curTime = glutGet(GLUT_ELAPSED_TIME);
        if (curTime - lastMoveTime < moveTimeout) {
            return;
        } else if(!isPlaying) {
            return;
        }

        Vector3 moveVec = Vector3(x, 0, z);
        Vector3 tempPos = player->position + moveVec;
        if (tempPos.z < topVerticalLimitPlayerPos || tempPos.z > bottomVerticalLimitPlayerPos ||
           tempPos.x > rightHorizontalLimitPlayerPos || tempPos.x < leftHorizontalLimitPlayerPos) {
            return;
        }

        lastMoveTime = curTime;
        player->jump(moveVec, 1.0f);
    }

    GLuint setupShaders() {
        // Shader for models
        shader.init();
        shader.loadShader(VSShaderLib::VERTEX_SHADER, VERTEX_SHADER_PATH);
        shader.loadShader(VSShaderLib::FRAGMENT_SHADER, FRAGMENT_SHADER_PATH);

        // Set semantics for the shader variables
        glBindFragDataLocation(shader.getProgramIndex(), 0,"colorOut");
        glBindAttribLocation(shader.getProgramIndex(), VERTEX_COORD_ATTRIB, "position");
        glBindAttribLocation(shader.getProgramIndex(), NORMAL_ATTRIB, "normal");
        glBindAttribLocation(shader.getProgramIndex(), TEXTURE_COORD_ATTRIB, "texCoord");
        glBindAttribLocation(shader.getProgramIndex(), VERTEX_ATTRIB1, "vVertex");
        glBindAttribLocation(shader.getProgramIndex(), VERTEX_ATTRIB2, "vtexCoord");
        glBindAttribLocation(shader.getProgramIndex(), TANGENT_ATTRIB, "tangent");
        glLinkProgram(shader.getProgramIndex());

        // Different modes of texturing
        texMode_uniformId = glGetUniformLocation(shader.getProgramIndex(), "texMode");
        pvm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_pvm");
        vm_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_viewModel");
        normal_uniformId = glGetUniformLocation(shader.getProgramIndex(), "m_normal");

        tex_road_loc = glGetUniformLocation(shader.getProgramIndex(), "tex_road");      //texmap0
        tex_river_loc = glGetUniformLocation(shader.getProgramIndex(), "tex_river");    //texmap1
        tex_grass_loc = glGetUniformLocation(shader.getProgramIndex(), "tex_grass");    //texmap2
        tex_text_loc = glGetUniformLocation(shader.getProgramIndex(), "tex_text");      //texmap3
        tex_tree_loc = glGetUniformLocation(shader.getProgramIndex(), "tex_tree");      //texmap4
        tex_particle_loc = glGetUniformLocation(shader.getProgramIndex(), "tex_particle");

        // Get the light indexes
        for (int i = 0; i < 8; i++) {
            std::string pos = "l_pos_" + to_string(i);
            l_pos_id[i] = glGetUniformLocation(shader.getProgramIndex(), pos.c_str());
        }

        // Get the index of a light boolean mask
        l_enabled_id = glGetUniformLocation(shader.getProgramIndex(), "l_enabled");
        l_spot_dir_id = glGetUniformLocation(shader.getProgramIndex(), "l_spot_dir");

        // Get the id of fog_enabled
        fog_enabled_id = glGetUniformLocation(shader.getProgramIndex(), "fogEnabled");

        printf("InfoLog for Per Fragment Phong Lightning Shader\n%s\n\n", shader.getAllInfoLogs().c_str());
        return(shader.isProgramLinked());
    }

    void initScene() {
        srand(time(NULL));

        // Texture Object definition
        glGenTextures(6, TextureArray);
        string road_text = "textures/Road.tga";
        string river_text = "textures/River.tga";
        string grass_text = "textures/Grass.tga";
        string anno_text = "textures/Anno_16x16_2.tga";
        string tree_text = "textures/tree.tga";
        string particle_text = "textures/particle.tga";

        TGA_Texture(TextureArray, (char*)road_text.c_str(), ROAD_TEXTURE_INDEX);
        TGA_Texture(TextureArray, (char*)river_text.c_str(), RIVER_TEXTURE_INDEX);
        TGA_Texture(TextureArray, (char*)grass_text.c_str(), GRASS_TEXTURE_INDEX);
        TGA_Texture(TextureArray, (char*)anno_text.c_str(), TEXT_TEXTURE_INDEX);
        TGA_Texture(TextureArray, (char*)tree_text.c_str(), TREE_TEXTURE_INDEX);
        TGA_Texture(TextureArray,  (char*)particle_text.c_str(), PARTICLE_TEXTURE_INDEX);

        createVehicles<Bus>(busses, 4, 1, 1, true, 80, 50);
        createVehicles<Car>(cars, 5, 3, 1, false, 80, 50);
        createVehicles<Car>(cars, 5, 5, 1, true, 80, 50);
        createVehicles<Log>(logs, 4, -1, 0, false, 60, 30);
        createVehicles<Log>(logs, 4, -3, 0, false, 60, 30);
        createVehicles<Log>(logs, 4, -5, 0, false, 60, 30);
        createVehicles<Turtle>(turtles, 2, -2, 0, true, 30, 20);
        createVehicles<Turtle>(turtles, 2, -4, 0, true, 30, 20);

        // Initialize all of the GameObjects
        for (GameObject* go : gameObjects) {
            go->init();
        }

        for (auto tree: trees) {
            tree->init();
        }

        stencil->init();

        // some GL settings
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);
        glEnable(GL_MULTISAMPLE);
        // Sets the background color of the game
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);

        // Setup the stencil
        if (USE_STENCIL) {
            glClearStencil(0x0);
            glEnable(GL_STENCIL_TEST);
        }
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

        //Just initialize with something for now, the tex coords are updated for each character printed
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

        // Set the orthographic projection matrix
        // ortho(0.0f, float(WinX), 0.0f, float(WinY), -1.0f, 1.0f);
    }

    void DrawString(float x, float y, const std::string& str) {
        float text_texCoords[8];
        pushMatrix(MODEL);
        translate(MODEL, x, y, 0);
        glBindVertexArray(text_vaoID);

        // Position our text
        // glTranslatef(x, y, 0.0);

        for (std::string::size_type i = 0; i < str.size(); ++i)
        {
            const float aux = 1.0f / 16.0f;

            int ch = int(str[i]);
            float xPos = float(ch % 16) * aux;
            float yPos = float(ch / 16) * aux;

            text_texCoords[0] = xPos;
            text_texCoords[1] = 1.0f - yPos - aux;
            text_texCoords[2] = xPos + aux;
            text_texCoords[3] = 1.0f - yPos - aux;
            text_texCoords[4] = xPos + aux;
            text_texCoords[5] = 1.0f - yPos - 0.001f;
            text_texCoords[6] = xPos;
            text_texCoords[7] = 1.0f - yPos - 0.001f;

            glBindBuffer(GL_ARRAY_BUFFER, text_texCoordBuffer);
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(float) * 8, &text_texCoords[0]);

            computeDerivedMatrix(PROJ_VIEW_MODEL);
            glUniformMatrix4fv(pvm_uniformId, 1, GL_FALSE, mCompMatrix[PROJ_VIEW_MODEL]);

            glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

            translate(MODEL, _fontSize * 0.8f, 0.0f, 0.0f);
        }
        glBindVertexArray(0);
        popMatrix(MODEL);

        glEnable(GL_DEPTH_TEST);
    }

    void renderScene() {
        FrameCount++;
        genDeltaTime();

        // Clear the buffer and load indentity into VIEW and MODEL matrix
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

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

        // Use the ShaderLib and print a debug string if needed
        glUseProgram(shader.getProgramIndex());
        if (!shader.isProgramValid()) {
            printf("Program Not Valid!\n");
            exit(1);
        }

        // Update SpotLight position
        spotLight->position = player->position + Vector3(0.5f, 1.2f, 0.5f);
        spotLight->light_dir = Vector3(0, 0, -1);

        // Update the fog state
        updateFog();

        // Bind the textures
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, TextureArray[ROAD_TEXTURE_INDEX]);
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, TextureArray[RIVER_TEXTURE_INDEX]);
        glActiveTexture(GL_TEXTURE2);
        glBindTexture(GL_TEXTURE_2D, TextureArray[GRASS_TEXTURE_INDEX]);
        glActiveTexture(GL_TEXTURE3);
        glBindTexture(GL_TEXTURE_2D, TextureArray[TEXT_TEXTURE_INDEX]);
        glActiveTexture(GL_TEXTURE4);
        glBindTexture(GL_TEXTURE_2D, TextureArray[TREE_TEXTURE_INDEX]);
        glActiveTexture(GL_TEXTURE5);
        glBindTexture(GL_TEXTURE_2D, TextureArray[PARTICLE_TEXTURE_INDEX]);

        // Send the textures
        glUniform1i(tex_road_loc, ROAD_TEXTURE_INDEX);
        glUniform1i(tex_river_loc, RIVER_TEXTURE_INDEX);
        glUniform1i(tex_grass_loc, GRASS_TEXTURE_INDEX);
        glUniform1i(tex_text_loc, TEXT_TEXTURE_INDEX);
        glUniform1i(tex_tree_loc, TREE_TEXTURE_INDEX);
        glUniform1i(tex_particle_loc, PARTICLE_TEXTURE_INDEX);

        // Check for boundaries of the moving objects
        checkVehicleBoundaryCollisions<Bus>(busses);
        checkVehicleBoundaryCollisions<Car>(cars);
        checkVehicleBoundaryCollisions<Log>(logs);
        checkVehicleBoundaryCollisions<Turtle>(turtles);

        // Types of collision determine whether the player has died or not
        bool riverBorder = false;
        bool roadDeath = false;
        bool hitRiver = false;
        bool hitLog = false;

        #define GAME_INVERSE_SPEED 200

        for (GameObject *go : gameObjects) {
            if (go->isEnabled()) {
                // Update the physics
                if (isPlaying){
                    go->update(float(deltaTime/float(GAME_INVERSE_SPEED)));
                }

                if (go->position != player->position) {
                    checkPlayerCollisions(go, riverBorder, roadDeath, hitRiver, hitLog);
                }

                // Render the objects ()
                if (!go->isTransparent) {
                    go->render();
                }
            }
        }

        // Render transparent objects
        for (GameObject *go : gameObjects) {
            if (go->isEnabled()) {
                if (go->isTransparent){
                    go->render();
                }
            }
        }

        // Render the trees as billboards - they need the camera position for this
        for (auto tree: trees) {
            if (currentCameraType == CAMERA_PERSPECTIVE_FOLLOW) {
                tree->render(cameraPerspectiveMoving.pos + cameraPerspectiveMoving.localPos);
            } else if (currentCameraType == CAMERA_PERSPECTIVE_FIXED) {
                tree->render(cameraPerspectiveFixed.pos + cameraPerspectiveFixed.localPos);
            }
        }

        bool deathInRiver = hitRiver && (!hitLog) && (player->playerState != ONLOG);

        if (riverBorder) {
            cout << "Death in river border!" << endl;
            onDeath();
        } else if (deathInRiver) {
            cout << "Death in river!" << endl;
            onDeath();
        } else if (roadDeath) {
            cout << "Death on the road!" << endl;
            onDeath();
        }

        // Swap the back and front buffer
        /*
        // HUD STUFF
        glBindTexture(GL_TEXTURE_2D, 0);
        glBlendFunc(GL_ONE, GL_ZERO);
        glDisable(GL_BLEND);
        // H U D
        glDisable(GL_DEPTH_TEST);
        pushMatrix(MODEL);
        loadIdentity(VIEW);
        loadIdentity(MODEL);
        loadIdentity(PROJECTION);
        ortho(0, 1280, 0, 1280, 0, 1);
        glUniform1i(texMode_uniformId, 3);

        _fontSize = 24;
        initTextureMappedFont();
        std::string s = "LIVES:dd a da sd sa dsa dashjdhsakdhsakjh" ;
        DrawString(10, 10, s);
        //popMatrix(MODEL);
        glEnable(GL_DEPTH_TEST);
        */

        // TODO: Remove from resize
        loadIdentity(PROJECTION);

        if (USE_STENCIL) {
            stencil->render();
        }

        selectCamera(currentCameraType);

        // Swap the buffers
        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        glDisable(GL_BLEND);
        glutSwapBuffers();
    }

private:
    void checkPlayerCollisions(GameObject *go, bool &riverBorder, bool &roadDeath, bool &hitRiver, bool &hitLog) {
        if (player->collideWith(go)) {
            if (go->getType() == BUS || go->getType() == CAR) {
                roadDeath = true;
            } else if(go->getType() == TARGET){
                target->setRandomPosition();
                increaseSpeedMultipliers();
                player->respawn();
                score += pointsPerTarget;
            } else if (go->getType() == DEADLYBOUNDS) {
                riverBorder = true;
            }
        } else if ((player->playerState == GROUNDED) && (player->collideWithBottom(go))) {
            //cout << "Bottom collision with " << go->getType() << endl;
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
            } else if (go->getType() == GROUND) {
                hitRiver = false;
            }
        } else if (player->playerState == ONTURTLE){
            if (go->getType() == TURTLE && (player->collideWithBottom(go))) {
                Turtle* turt = (Turtle*) go;
                if(turt->isUnderWater){
                    hitRiver = true;
                    player->playerState = GROUNDED;
                }
            }
        }
    }

    void onDeath(){
        player->currentLives--;

        if (player->currentLives == 0) {
            infoString = "Player has died! The achieved score: " + to_string(score);
            isPlaying = false;
        }
        player->respawn();
    }

    void genDeltaTime() const {
        GLint currentTime = glutGet(GLUT_ELAPSED_TIME);
        deltaTime = prevTime - currentTime;
        prevTime = currentTime;
    }

    void resetSpeedMultipliers() {
        for (auto &go: gameObjects) {
            go->setSpeedMultiplier(1.0f);
        }
    }

    void increaseSpeedMultipliers() {
        for (auto &go: gameObjects) {
            go->setSpeedMultiplier(go->getSpeedMultiplier()*speedMultiplier);
        }
    }

    void changeAnimationState(bool newState) {
        for (auto &go: gameObjects) {
            go->setAnimationEnabled(newState);
        }
    }

    void selectCamera(CameraType newCamera) {
        loadIdentity(PROJECTION);
        currentCameraType = newCamera;

        if (newCamera == CAMERA_PERSPECTIVE_FOLLOW) {
            cameraPerspectiveMoving.project(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        } else if (newCamera == CAMERA_PERSPECTIVE_FIXED) {
            cameraPerspectiveFixed.project(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        } else if (newCamera == CAMERA_ORTHO) {
            cameraOrthogonal.project(glutGet(GLUT_WINDOW_WIDTH), glutGet(GLUT_WINDOW_HEIGHT));
        }
    }

    template <typename T>
    void createVehicles(vector<T*> &vehicleVector, int n, float zPos, float yPos, bool isGoingRight, int randMod, int randConst) {
        float randSpeed = (float)(rand() % randMod + randConst) / 100.0f;
        for (int j = 0; j < n; j++) {
            int offset = rand() % 7 + 1;

            Vector3 spawnPosition;
            Vector3 spawnSpeed;

            if (isGoingRight){
                spawnPosition = Vector3(-7.0f - offset, yPos, zPos);
                spawnSpeed = Vector3(-randSpeed, 0, 0);
                if (j > 0){
                    spawnPosition.x = vehicleVector.back()->position.x - vehicleVector.back()->boundingBox.vecMax.x - 0.5f - offset;
                }
            } else {
                spawnPosition = Vector3(7.0f + offset, yPos, zPos);
                spawnSpeed = Vector3(randSpeed, 0, 0);
                if (j > 0){
                    spawnPosition.x = vehicleVector.back()->position.x + vehicleVector.back()->boundingBox.vecMax.x  +0.5f + offset;
                }
            }

            T* vehicle = new T(spawnPosition, spawnSpeed, isGoingRight);
            if (vehicle->getType() == TURTLE){
                // cout << vehicle->isGoingRight << endl;
                // cout << isGoingRight << endl;
            }
            // Add the vehicle to both the GameObjects vector as well as the vehicle-specific vector vehicleVector
            vehicleVector.push_back(vehicle);
            gameObjects.push_back(vehicle);
        }
    }

    template <typename T>
    void checkVehicleBoundaryCollisions(vector<T*> &vehicleVector) {
        const float minX = -6;
        const float maxX = 7;
        for (T* vehicle: vehicleVector) {
            /*if(vehicle->getType() == TURTLE){
                cout << vehicle->isGoingRight << endl;
            }*/
            if (vehicle->isGoingRight) {
                if (((vehicle->position.x + vehicle->boundingBox.vecMin.x) > maxX)
                    && ((vehicle->position.x + vehicle->boundingBox.vecMax.x) > maxX)) {
                    vehicle->respawn();
                    int offset = rand() % 3 + 1;
                    vehicle->position.x -= offset;

                    while(checkVehicleCollision(vehicle, vehicleVector)){
                        if(vehicle->getType() == TURTLE){
                            cout << "Turtles collided" << endl;
                        }
                        vehicle->position.x -= (vehicle->boundingBox.vecMax.x + 0.5);
                    }
                }
            } else if (!vehicle->isGoingRight) {
                if (((vehicle->position.x + vehicle->boundingBox.vecMin.x) < minX)
                    && ((vehicle->position.x + vehicle->boundingBox.vecMax.x) < minX)) {
                    vehicle->respawn();
                    int offset = rand() % 3 + 1;
                    vehicle->position.x += offset;
                    while(checkVehicleCollision(vehicle, vehicleVector)){
                        vehicle->position.x += vehicle->boundingBox.vecMax.x + 0.5;
                    }
                }
            }
        }
    }

    template <typename T>
    bool checkVehicleCollision(T* primeVehicle, vector<T*> &vehicleVector){
        for (T* vehicle: vehicleVector) {
            if (vehicle->position != primeVehicle->position) {
                if (primeVehicle->collideWith(vehicle)){
                    return true;
                }
            }
        }
        return false;
    }

    void updateFog() {
        //  Set the state of the fog and send it into the shader
        glUniform1i(fog_enabled_id, fog_enabled);
    }
};

#endif //AVT7_GAMEMANAGER_H