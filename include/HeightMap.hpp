// HeightMap.hpp
// Note: This code was borrowed from the sample (8) code.
#pragma once

#include "Image.hpp"

#include <string>
#include <glad/glad.h>

class HeightMap {
    public:
        HeightMap();
        HeightMap(std::string filePath);

        ~HeightMap();

        int getWidth();

        int getDepth();

        // Loads and sets up a HeightMap.
        void LoadHeightMap(std::string fileName);

        Image* getImage();

    private:
        std::string m_filePath;
        int m_width{0};
        int m_height{0};
        Image* m_image;
};
