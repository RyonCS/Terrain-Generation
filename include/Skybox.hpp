#pragma once

#include "Texture.hpp"
#include "Terrain.hpp"
#include <iostream>

class Skybox {
    public:
        Skybox();
        Skybox(std::string filePath);

        ~Skybox();

        Texture* getSkyboxTexture(int index);

        // std::vector<float>& getSkyboxVertices();

        // std::vector<float>& getSkyboxIndices();

        void Initialize();

        void generateSkyboxCube();

        void loadSkyboxTextures();

        void PreDraw();

        void Draw();

        void Scale(int scaleAmount);

        void Translate(glm::vec3 translateVec);

        float* getSkyboxVertices();


    private:
        std::vector<Texture*> m_SkyboxTextures; // front, back, left, right, top, bottom
        std::vector<std::string> m_skyboxTexturePaths;
        
        float m_skyboxVertices[144] = {       
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,

        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,

        -1.0f,  1.0f, -1.0f,
        1.0f,  1.0f, -1.0f,
        1.0f,  1.0f,  1.0f,
        1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,

        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f, -1.0f,
        1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
        1.0f, -1.0f,  1.0f
        };

        Texture m_skyboxTexture;

        GLuint m_skyboxVAO{0};
        GLuint m_skyboxVBO{0};
        GLuint m_skyboxIBO{0};
        GLuint mShaderID;
};