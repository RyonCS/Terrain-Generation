// Image.cpp
// Handles reading in a PPM file.

#include "Image.hpp"
#include <fstream>
#include <sstream>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <memory>

Image::Image(std::string filePath) : m_filePath(filePath){}

Image::~Image() {
    if (m_pixelData != NULL) {
        delete[] m_pixelData;
        m_pixelData = nullptr;
    }
}

// Parses a PPM file and stores the pixel data.
bool Image::LoadPPM(bool flip) {
    std::ifstream ppmFile(m_filePath);
    std::string line;

    if (!ppmFile.is_open()) {
        std::cout << "Error Reading in ppm file: " << m_filePath << std::endl;
        return false;
    }

    // First get the magic number (P3 or P6 usually).
    std::getline(ppmFile, magicNumber);

    // Get the height, width, and max color range while ignoring comments.
    while(std::getline(ppmFile, line)) {
        if (line.empty() || line[0] == '#') {
            continue;
        }
        std::istringstream iss(line);
        if (m_width == 0 && m_height == 0) {
            iss >> m_height;
            iss >> m_width;
        } else if (m_maxColorRange == 0) {
            iss >> m_maxColorRange;
            break;
        }
    }

    // Start storing pixel data.
    m_pixelData = new uint8_t[m_width * m_height * 3];
    int index = 0;
    int r, g, b;
    // Put the next 3 numbers as RGB and repeat until end of file.
    while(index < m_width * m_height * 3 && ppmFile >> r >> g >> b) {
        m_pixelData[index++] = static_cast<uint8_t>(r);
        m_pixelData[index++] = static_cast<uint8_t>(g);
        m_pixelData[index++] = static_cast<uint8_t>(b);
    }

    ppmFile.close();

    // Flip all of the pixels
    if(flip){
        // Copy all of the data to a temporary stack-allocated array
        uint8_t* copyData = new uint8_t[m_width*m_height*3];
        for(int i =0; i < m_width*m_height*3; ++i){
            copyData[i]=m_pixelData[i];
        }
        
        unsigned int pos = (m_width*m_height*3)-1;
        for(int i =0; i < m_width*m_height*3; i+=3){
            m_pixelData[pos]=copyData[i+2];
            m_pixelData[pos-1]=copyData[i+1];
            m_pixelData[pos-2]=copyData[i];
            pos-=3;
        }
        delete[] copyData;
    }
    return true;
}

// Set a pixels coordinates and color.
void Image::SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b) {
    if (x > m_width || y > m_height) {
        return;
    } else {
        m_pixelData[(x * 3) + m_height * (y*3)] = r;
        m_pixelData[(x * 3) + m_height * (y*3) + 1] = g;
        m_pixelData[(x * 3) + m_height * (y*3) + 2] = b;
    }
}

// Prints all pixel data.
void Image::PrintPixels() {
    for (int i = 0; i < m_width * m_height * 3; ++i) {
        std::cout << " " << (int)m_pixelData[i];
    }
    std::cout << "\n";
}

// Gets a pointer to our pixels.
uint8_t* Image::GetPixelDataPtr() {
    return m_pixelData;
}