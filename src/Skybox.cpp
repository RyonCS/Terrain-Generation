// Skybox.cpp
// Handles generating a skybox.
#include "Skybox.hpp"

#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include <glad/glad.h>
#include "Shader.hpp"
#include <glm/mat4x4.hpp>
#include <glm/gtc/matrix_transform.hpp> 

#include "Globals.hpp"

Skybox::Skybox() {};

Skybox::Skybox(std::string filePath) {
    Texture* skyboxTexture = new Texture(filePath);
    m_SkyboxTextures.push_back(skyboxTexture);
}

Skybox::~Skybox() {};

// Gets a skybox texture given an index.
Texture* Skybox::getSkyboxTexture(int index) {
    return m_SkyboxTextures[index];
}

// Initializes skybox -- create shader, send vertices and skybox texture to GPU.
void Skybox::Initialize() {
    std::string vertexShaderSource      = LoadShaderAsString("./shaders/skybox_vert.glsl");
    std::string fragmentShaderSource    = LoadShaderAsString("./shaders/skybox_frag.glsl");

    mShaderID = CreateShaderProgram(vertexShaderSource,fragmentShaderSource);

    // Sends vertices to GPU.
    generateSkyboxCube();
    
    // Creates skybox texture and sends to GPU.
    loadSkyboxTextures();
}

// Send uniforms to GPU
void Skybox::PreDraw() {
    glUseProgram(mShaderID);
    glDepthFunc(GL_LEQUAL);

    // View Matrix.
    glm::mat4 viewMatrix = glm::mat4(glm::mat3(g.gCamera.GetViewMatrix()));

    // Projection Matrix.
    glm::mat4 projection = glm::perspective(glm::radians(75.0f),(float)g.gScreenWidth/(float)g.gScreenHeight, 0.1f,100.0f);

    // Send View Matrix uniform to shader.
    GLint view_location = glGetUniformLocation(mShaderID,"view");
    if(view_location >=0 ){
        glUniformMatrix4fv(view_location, 1, GL_FALSE, &viewMatrix[0][0]);
    }else {
        std::cout << "Could not find view, maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }

    // Send Projection Matrix uniform to shader.
    GLint projection_location = glGetUniformLocation(mShaderID,"projection");
    if(projection_location >=0 ){
        glUniformMatrix4fv(projection_location, 1, GL_FALSE, &projection[0][0]);
    }else {
        std::cout << "Could not find projection, maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }

    // Activate and bind skybox.
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture.getTextureID());

    // Send skybox texture to GPU.
    GLint skybox_location = glGetUniformLocation(mShaderID, "skybox");
    if (skybox_location >= 0) {
        glUniform1i(skybox_location, 0);
    } else {
        std::cout << "Could not find skybox uniform in shader program.\n";
        exit(EXIT_FAILURE);
    }
}

// Draws the skybox.
void Skybox::Draw() {
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_skyboxTexture.getTextureID());

    glDisable(GL_CULL_FACE);
    glDepthMask(GL_FALSE);
    glDepthFunc(GL_LEQUAL);

    glBindVertexArray(m_skyboxVAO);
    glDrawArrays(GL_TRIANGLES, 0, 36);
    
    glDepthMask(GL_TRUE);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);
    glBindVertexArray(0);
    glUseProgram(0);
}

// Sends skybox vertices to GPU.
void Skybox::generateSkyboxCube() {

    // Scale the size of the skybox by 10000.
    Scale(10000);
    
    // Translate the skybox to surround the center of the terrain.
    glm::vec3 centerOfTerrain = g.gTerrain.getCenterOfTerrain();
    Translate(centerOfTerrain);

    // -- Send to GPU --
    float* skyboxVertices = g.gSkybox.getSkyboxVertices();
    int numSkyboxVertices = 144;
    
    glGenVertexArrays(1, &m_skyboxVAO);
    glBindVertexArray(m_skyboxVAO);

    glGenBuffers(1, &m_skyboxVBO);
    glBindBuffer(GL_ARRAY_BUFFER, m_skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(m_skyboxVertices), m_skyboxVertices, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Load textures into skybox texture,.
void Skybox::loadSkyboxTextures() {
    m_skyboxTexturePaths.push_back("./thirdparty/resources/skybox/skybox_right_1024.ppm");
    m_skyboxTexturePaths.push_back("./thirdparty/resources/skybox/skybox_left_1024.ppm");
    m_skyboxTexturePaths.push_back("./thirdparty/resources/skybox/skybox_top_1024.ppm");
    m_skyboxTexturePaths.push_back("./thirdparty/resources/skybox/skybox_bottom_1024.ppm");
    m_skyboxTexturePaths.push_back("./thirdparty/resources/skybox/skybox_front_1024.ppm");
    m_skyboxTexturePaths.push_back("./thirdparty/resources/skybox/skybox_back_1024.ppm");

    m_skyboxTexture.loadSkyboxTextures(m_skyboxTexturePaths);
}

// Scale the size of the skybox.
void Skybox::Scale(int scaleAmount) {
    int numVertices = sizeof(m_skyboxVertices) / sizeof(m_skyboxVertices[0]);

    for (int i = 0; i < numVertices ; i++) {
        m_skyboxVertices[i] *= scaleAmount;    
    }
}

// Translate the skybox.
void Skybox::Translate(glm::vec3 translateVec) {
    int numVertices = sizeof(m_skyboxVertices) / sizeof(m_skyboxVertices[0]);

    for (int i = 0; i < numVertices ; i += 3) {
        m_skyboxVertices[i] += translateVec.x;
        m_skyboxVertices[i + 1] += translateVec.y; 
        m_skyboxVertices[i + 2] += translateVec.z;   
    }
}

// Returns the skybox vertices.
float* Skybox::getSkyboxVertices() {
    return m_skyboxVertices;
}