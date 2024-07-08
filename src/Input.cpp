// Input.cpp
// Handles user input.
#include "Input.hpp"
#include "Globals.hpp"

void Input() {
    // Two static variables to hold the mouse position.
    static int mouseX = g.gScreenWidth/2;
    static int mouseY = g.gScreenHeight/2;

    // Event handler for various SDL events related to input and output.
    SDL_Event e;
    // Handle events on queue.
    while(SDL_PollEvent(&e) != 0) {
        if(SDL_QUIT == e.type) {
            std::cout << "Goodbye! (Leaving MainApplicationLoop())" << std::endl;
            g.gQuit = true;
        }
        // Exit program on ESC or red button.
        if (SDL_KEYDOWN == e.type && SDLK_ESCAPE == e.key.keysym.sym) {
            std::cout << "Goodbye! (Leaving MainApplicationLoop())" << std::endl;
            g.gQuit = true;
        } 
        // On mouse movement rotate around up vector.
        if (e.type == SDL_MOUSEMOTION) {
            mouseX += e.motion.xrel / 10;
            mouseY += e.motion.yrel / 10;
            g.gCamera.MouseLook(mouseX, mouseY);
        }
    }

    // Retrieve keyboard state and constrain camera to Terrain surface.
    const Uint8 *state = SDL_GetKeyboardState(NULL);
    // Forward Camera.
    if (state[SDL_SCANCODE_W]) {
        g.gCamera.MoveForward(.005f);
        g.gTerrain.ConstrainCameraToTerrain();
    }
    // Backward Camera.
    if (state[SDL_SCANCODE_S]){
        g.gCamera.MoveBackward(0.005f);
        g.gTerrain.ConstrainCameraToTerrain();
    }
    // Left Camera.
    if (state[SDL_SCANCODE_A]) {
        g.gCamera.MoveLeft(0.005f);
        g.gTerrain.ConstrainCameraToTerrain();
    }
    // Right Camera.
    if (state[SDL_SCANCODE_D]) {
        g.gCamera.MoveRight(0.005f);
        g.gTerrain.ConstrainCameraToTerrain();
    }
    // Up Camera.
    if (state[SDL_SCANCODE_UP]) {
        g.gCamera.MoveUp(0.005f);
        g.gTerrain.ConstrainCameraToTerrain();
    }
    // Right Camera.
    if (state[SDL_SCANCODE_DOWN]) {
        g.gCamera.MoveDown(0.005f);
        g.gTerrain.ConstrainCameraToTerrain();
    }
    // "1" fills object.
    if (state[SDL_SCANCODE_1]) {
        SDL_Delay(250);
        if (GL_FILL == g.gPolygonMode) {
            g.gPolygonMode = GL_LINE;
        } else {
            g.gPolygonMode = GL_FILL;
        }
    }
    // "2" removes the object from the screen.
    if (state[SDL_SCANCODE_2]) {
        SDL_Delay(250);
        g.drawObject = !g.drawObject;
    }
}
