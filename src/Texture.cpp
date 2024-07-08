// Texture.cpp
// Handles loading in general textures and skybox textures.

#include <SDL2/SDL.h>

#include "Texture.hpp"

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <glad/glad.h>
#include <memory>

Texture::Texture() {}

Texture::Texture(std::string fileName) {
    LoadTexture(fileName);
}

Texture::~Texture() {
    // glDeleteTextures(1, &m_textureID);

    // if(nullptr != m_image) {
    //     delete m_image;
    // }
}

// Loads in a texture given a ppm file.
void Texture::LoadTexture(const std::string filePath) {
    m_filePath = filePath;

    m_image = new Image(filePath);
    m_image->LoadPPM(true);

    glEnable(GL_TEXTURE_2D);
    glGenTextures(1, &m_textureID);

    glBindTexture(GL_TEXTURE_2D, m_textureID);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, m_image->GetWidth(), m_image->GetHeight(), 0, 
                 GL_RGB, GL_UNSIGNED_BYTE, m_image->GetPixelDataPtr());

    glGenerateMipmap(GL_TEXTURE_2D);

    glBindTexture(GL_TEXTURE_2D, 0);
}

// Binds a texture.
void Texture::Bind(unsigned int slot) const {
    glEnable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0+slot);
    glBindTexture(GL_TEXTURE_2D, m_textureID);
}

// Unbinds a texture.
void Texture::Unbind() {
    glBindTexture(GL_TEXTURE, 0);
}

// Loads in a skybox texture.
void Texture::loadSkyboxTextures(std::vector<std::string>& fileNames) {
    glGenTextures(1, &m_textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, m_textureID);

    // Iterate through skybox textures and add to cubemap.
    int faceIndex = 0;
    for (const auto& fileName : fileNames) {
        m_image = new Image(fileName);
        if (!m_image->LoadPPM(false)) { 
            std::cerr << "Failed to load texture file: " << fileName << std::endl;
            continue;
        }

        // Adds texture to cubemap based on face index. Ordering of files matters.
        glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + faceIndex, 0, GL_RGB, m_image->GetWidth(), m_image->GetHeight(), 0,
                     GL_RGB, GL_UNSIGNED_BYTE, m_image->GetPixelDataPtr());
        faceIndex++;

        delete m_image;
    }
    
    // Sets cube map settings.
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    // Generates a mipmap of the skycube.
    glGenerateMipmap(GL_TEXTURE_CUBE_MAP);

    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);
}

// Returns the image of the texture.
Image* Texture::getImage() {
    return m_image;
}

// Returns the texture ID.
GLuint Texture::getTextureID() {
    return m_textureID;
}