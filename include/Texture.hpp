// Texture.hpp
// Note: This code was borrowed from the sample (8) code.
#pragma once

#include "Image.hpp"

#include <string>
#include <glad/glad.h>

class Texture {
    public:
        Texture();
        Texture(std::string filePath);

        ~Texture();

        // Loads and sets up a texture.
        void LoadTexture(std::string fileName);

        // Binds a texture.
        void Bind(unsigned int slot = 0) const;

        // Unbinds a texture.
        void Unbind();

        Image* getImage();

        void loadSkyboxTextures(std::vector<std::string>& fileNames);

        GLuint getTextureID();

    private:
        GLuint m_textureID;
        std::string m_filePath;
        Image* m_image;
};