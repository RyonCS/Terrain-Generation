// HeightMap.cpp
// Handles loading in a heightmap ppm file.

#include <SDL2/SDL.h>

#include "HeightMap.hpp"

#include <stdio.h>
#include <string.h>
#include <fstream>
#include <iostream>
#include <glad/glad.h>
#include <memory>

HeightMap::HeightMap() {}

HeightMap::HeightMap(std::string fileName) {
    LoadHeightMap(fileName);
}

HeightMap::~HeightMap() {
}

// Returns the width of a heightmap.
int HeightMap::getWidth() {
    return m_width;
}

// Returns the depth of the heightmap.
int HeightMap::getDepth() {
    return m_height;
}

// Loads in a heightmap.
void HeightMap::LoadHeightMap(const std::string filePath) {
    m_filePath = filePath;

    m_image = new Image(filePath);
    m_image->LoadPPM(true);
    m_height = m_image->GetHeight();
    m_width = m_image->GetWidth();
}

// Returns the image of the heightmap.
Image* HeightMap::getImage() {
    return m_image;
}