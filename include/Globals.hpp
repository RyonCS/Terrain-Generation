#pragma once

// -- THIS FILE ALLOWS ACCESS TO GLOBAL VARIABLES IN MAIN --

// Third Party Libraries
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/vec3.hpp>
#include <glm/vec4.hpp>
#include <glm/gtc/matrix_transform.hpp>

// C++ Standard Libraries
#include <iostream>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

// Other Libraries.
#include "Camera.hpp"
#include "Image.hpp"
#include "Terrain.hpp"
#include "Texture.hpp"
#include "HeightMap.hpp"
#include "Skybox.hpp"

struct Global {
    int gScreenWidth = 1024;
    int gScreenHeight = 640;
    SDL_Window* gGraphicsApplicationWindow = nullptr;
    SDL_GLContext gOpenGLContext = nullptr;
    bool gQuit;
    GLuint gGraphicsPipelineShaderProgram;
    GLenum gPolygonMode = GL_FILL;
    Camera gCamera;
    bool drawObject = true;
    HeightMap gHeightMap;
    Terrain gTerrain;
    GLuint gPlane_VAO = 0;
    GLuint gPlane_VBO = 0;
    GLuint gPlane_IBO = 0;
    Texture gTexture;
    Skybox gSkybox;
};

extern Global g;