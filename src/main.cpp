// main.cpp
// This file sets up our SDL, OpenGL State, initializes vertices and has a main loop for rendering the scene.

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
#include "Input.hpp"
#include "Shader.hpp"
#include "Texture.hpp"


// --- Setting Globals ---
# include "Globals.hpp"

// Set up our SDL window.
void InitializeProgram() {
    // Initialize SDL.
    if (SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "SDL could not be initialized. SDL Error: " << SDL_GetError() << "\n";
        exit(1);
    }

    // Get the dimensions of the screen
    SDL_DisplayMode displayMode;
    SDL_GetCurrentDisplayMode(0, &displayMode);

    // Set the window width and height to match the screen dimensions
    int screenWidth = displayMode.w;
    int screenHeight = displayMode.h;
    g.gScreenHeight = screenHeight;
    g.gScreenWidth = screenWidth;

    // Set up OpenGL Context.
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    // Request double buffer.
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);

    // Create an application window using OpenGL that supports SDL
    g.gGraphicsApplicationWindow = SDL_CreateWindow("OBJ Reader",
                                                  SDL_WINDOWPOS_UNDEFINED,
                                                  SDL_WINDOWPOS_UNDEFINED,
                                                  g.gScreenWidth,
                                                  g.gScreenHeight,
                                                  SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN);

    // Check window was created.
    if (nullptr == g.gGraphicsApplicationWindow) {
        std::cout << "Window could not be created, SDL error: " << SDL_GetError() << "\n";
        exit(1);
    }

    // Create an OpenGL Graphics Context
    g.gOpenGLContext = SDL_GL_CreateContext( g.gGraphicsApplicationWindow );
    if (nullptr == g.gOpenGLContext) {
        std::cout << "OpenGL context could not be created. SDL Error: " << SDL_GetError() << "\n";
        exit(1);
    }

    // Initialize GLAD Library
    if(!gladLoadGLLoader(SDL_GL_GetProcAddress)) {
        std::cout << "glad did not initialize" << "\n";
        exit(1);
    }

    std::string vertexShaderSource = LoadShaderAsString("./shaders/vert.glsl");
    std::string fragmentShaderSource = LoadShaderAsString("./shaders/frag.glsl");

    g.gGraphicsPipelineShaderProgram = CreateShaderProgram(vertexShaderSource, fragmentShaderSource);

    SDL_Log("(Note: If you have two GPU's, make sure the correct one is selected)");
	SDL_Log("Vendor: %s",(const char*)glGetString(GL_VENDOR));
	SDL_Log("Renderer: %s",(const char*)glGetString(GL_RENDERER));
	SDL_Log("Version: %s",(const char*)glGetString(GL_VERSION));
	SDL_Log("Shading language: %s",(const char*)glGetString(GL_SHADING_LANGUAGE_VERSION));
}

// Our main program loop.
void MainLoop() {
    SDL_WarpMouseInWindow(g.gGraphicsApplicationWindow, g.gScreenWidth/2, g.gScreenHeight/2);
    SDL_SetRelativeMouseMode(SDL_TRUE);

    while(!g.gQuit) {
        Input();
        glClear(GL_DEPTH_BUFFER_BIT | GL_COLOR_BUFFER_BIT);
        g.gSkybox.PreDraw();
        g.gSkybox.Draw();
        g.gTerrain.renderTerrain();
        SDL_GL_SwapWindow(g.gGraphicsApplicationWindow);
    }
}

// Cleans up our program.
void CleanUp() {
    SDL_DestroyWindow(g.gGraphicsApplicationWindow);
    g.gGraphicsApplicationWindow = nullptr;

    glDeleteBuffers(1, &g.gPlane_VBO);

    glDeleteVertexArrays(1, &g.gPlane_VAO);

    glDeleteProgram(g.gGraphicsPipelineShaderProgram);

    SDL_Quit();
}

// Main entry point into our program.
int main(int argc, char* argv[]) {
    // 1. Set up the graphics program.
    InitializeProgram();

    // Load height map file
    g.gHeightMap.LoadHeightMap(argv[1]);
    g.gTerrain.setHeightMap(g.gHeightMap);

    // Initialize vertices.
    g.gTerrain.Initialize();
    g.gSkybox.Initialize();

    // 4. Call the main application loop.
    MainLoop();

    // 5. Clean up.
    CleanUp();
    
    return 0;
}
