// Image.cpp
// Note: This code was borrowed from the sample (8) code.
#pragma once

#include <string>

class Image {
public:
    Image(std::string filePath);

    ~Image();

    // Parses a ppm file.
    bool LoadPPM(bool flip);

    // Returns the width of the texture.
    inline int GetWidth() {
        return m_width;
    }

    // Returns the height of the texture.
    inline int GetHeight() {
        return m_height;
    }

    // Returns the bits-per-pixel of the texture.
    inline int GetBPP() {
        return m_BPP;
    }

    // Sets a pixels location and color.
    void SetPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b);

    // Prints the pixel data.
    void PrintPixels();

    // Gets the pointer to the pixel data.
    uint8_t* GetPixelDataPtr();

    // Gets the pixel R color based on coordinate.
    inline unsigned int GetPixelR(int x, int y) {
        return m_pixelData[(x*3) + m_height * (y*3)];
    }

    // Gets the pixel G color based on coordinate.
    inline unsigned int GetPixelG(int x, int y) {
        return m_pixelData[(x*3) + m_height * (y*3) + 1];
    }

    // Gets the pixel B color based on coordinate.
    inline unsigned int GetPixelB(int x, int y) {
        return m_pixelData[(x*3) + m_height * (y*3) + 2];
    }

    // Returns the file path to the texture.
    inline std::string getFilePath() {
        return m_filePath;
    }

private:
    std::string m_filePath;

    uint8_t* m_pixelData;

    int m_width{0};
    int m_height{0};
    int m_BPP{0};
    int m_maxColorRange{0};
    std::string magicNumber;
};