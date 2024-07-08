// Terrain.cpp
// Handles generating a terrain based on a heightmap and texture sampling.

#include "Terrain.hpp"
#include <cassert>
#include <iostream>

#include "Globals.hpp"

Terrain::Terrain(){}

Terrain::~Terrain(){}

void Terrain::Initialize() {
    // Load in 4 textures to sample from on terrain.
    Texture water;
    water.LoadTexture(waterTexturePath);
    Texture grass;
    grass.LoadTexture(grassTexturePath);
    Texture rock;
    rock.LoadTexture(rockTexturePath);
    Texture snow;
    snow.LoadTexture(snowTexturePath);

    g.gTerrain.addTexture(water);
    g.gTerrain.addTexture(grass);
    g.gTerrain.addTexture(rock);
    g.gTerrain.addTexture(snow);

    // Generate a plane the same size of our heightmap.
    g.gTerrain.generatePlane();
    std::cout << "Plane generated." <<std::endl;

    // Calculate normals for light calculations.
    CalculateNormals();

    // Get terrain vertices (y values have been modified by height map.)
    std::vector<Vertex> vertices = g.gTerrain.getVertices();
    std::vector<unsigned int> triangleIndices = g.gTerrain.getTriangleIndices();

    // Vertex Arrays Object (VAO) Setup
    glGenVertexArrays(1, &g.gPlane_VAO);
    glBindVertexArray(g.gPlane_VAO);

    // Vertex Buffer Object (VBO) Setup
    glGenBuffers(1, &g.gPlane_VBO);
    glBindBuffer(GL_ARRAY_BUFFER, g.gPlane_VBO);
    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), vertices.data(), GL_STATIC_DRAW);

    // Vertice coordinates.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Texture coordinates.
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(3 * sizeof(float)));

    // Normals.
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)(5 * sizeof(float)));

    // Index Buffer Object (IBO) Setup
    glGenBuffers(1, &g.gPlane_IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g.gPlane_IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(triangleIndices[0]) * triangleIndices.size(),
                 triangleIndices.data(),
                 GL_STATIC_DRAW);

    // Unbind.
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

// Sets a loaded heightmap to terrain and gathers key stats: height, width, and terrain size.
void Terrain::setHeightMap(HeightMap heightMap) {
    m_heightMap = heightMap;
    m_width = m_heightMap.getWidth();
    m_depth = m_heightMap.getDepth();
    m_terrainSize = m_heightMap.getWidth();
}

// Returns the heightmap.
HeightMap Terrain::getHeightMap() {
    return m_heightMap;
}

// Returns the list of Vertexes.
std::vector<Vertex>& Terrain::getVertices() {
    return m_vertices;
}

// Returns triangle indices.
std::vector<unsigned int>& Terrain::getTriangleIndices() {
    return m_triangleIndices;
}

// Returns the depth of the terrain.
int Terrain::getDepth() {
    return m_depth;
}

// Returns the width of the terrain.
int Terrain::getWidth() {
    return m_width;
}

// Generates a plane of Vertexes with vertice and texture coordinates.
void Terrain::generatePlane() {
    // Resize structure to hold the correct amount of data.
    m_vertices.resize(m_width * m_depth);
    // Get heightmap pixel data.
    Image* pixel_data = getHeightMap().getImage();

    float m_worldScale = 1.0f;
    float terrainSize = getTerrainSize();

    int vertex_index = 0;

    for (int x = 0; x < m_width; x++) {
        for (int z = 0; z < m_depth; z++) {
            // First assign x, y, z values to the Vertex.
            assert(vertex_index < m_vertices.size());

            // Get y value by taking RGB and getting the average.
            uint8_t red = pixel_data->GetPixelR(x, z);
            uint8_t green = pixel_data->GetPixelG(x, z);
            uint8_t blue = pixel_data->GetPixelB(x, z);
            float height = (red + green + blue) / 3.0;
            
            // Assign plane coordinates, texture coordinates, 
            m_vertices[vertex_index].x = x * m_worldScale;
            m_vertices[vertex_index].z = z * m_worldScale;
            m_vertices[vertex_index].y = height * m_worldScale;
            m_vertices[vertex_index].u = (float)x / terrainSize;
            m_vertices[vertex_index].v = (float)z / terrainSize;
            
            vertex_index++;
        }
    }
    // Prepare for generating the triangle indices based on the Vertexes above.
    int numQuads = (getWidth() - 1) * (getDepth() - 1);
    getTriangleIndices().resize(numQuads * 6);

    std::vector<unsigned int>& triangleIndices = getTriangleIndices();
    InitIndices(triangleIndices);
}

// Generates indices for the triangles in our plane.
void Terrain::InitIndices(std::vector<unsigned int>& indices) {
    int index = 0;

    // Generate two triangle indices per set of 4 points on our plane.
    for (int z = 0; z < m_depth - 1; z++) {
        for (int x = 0; x < m_width - 1; x++) {
            unsigned int bottomLeft = z * m_width + x;
            unsigned int topLeft = (z + 1) * m_width + x;
            unsigned int topRight = (z + 1) * m_width + x + 1;
            unsigned int bottomRight = z * m_width + x + 1;

            // Add top left triangle.
            indices[index++] = bottomLeft;
            indices[index++] = topLeft;
            indices[index++] = topRight;

            // Add bottom right triangle.
            indices[index++] = bottomLeft;
            indices[index++] = topRight;
            indices[index++] = bottomRight;
        }
    }
}

// Calculate normal vectors for each Vertex.
void Terrain::CalculateNormals() {
    unsigned int index = 0;

    for (unsigned int i = 0; i < m_triangleIndices.size(); i += 3) {
        unsigned int index0 = m_triangleIndices[i];
        unsigned int index1 = m_triangleIndices[i + 1];
        unsigned int index2 = m_triangleIndices[i + 2];

        glm::vec3 v1 = glm::vec3{m_vertices[index1].x, m_vertices[index1].y, m_vertices[index1].z} -
                       glm::vec3{m_vertices[index0].x, m_vertices[index0].y, m_vertices[index0].z};

        glm::vec3 v2 = glm::vec3{m_vertices[index2].x, m_vertices[index2].y, m_vertices[index2].z} -
                       glm::vec3{m_vertices[index0].x, m_vertices[index0].y, m_vertices[index0].z};

        glm::vec3 normal = cross(v1, v2);
        normal = normalize(normal);

        m_vertices[index0].normal_x = normal.x;
        m_vertices[index0].normal_y = normal.y;
        m_vertices[index0].normal_z = normal.z;

        m_vertices[index1].normal_x = normal.x;
        m_vertices[index1].normal_y = normal.y;
        m_vertices[index1].normal_z = normal.z;

        m_vertices[index2].normal_x = normal.x;
        m_vertices[index2].normal_y = normal.y;
        m_vertices[index2].normal_z = normal.z;
    }
}

// Add terrain textures to structure.
void Terrain::addTexture(Texture& texture) {
    m_textures.push_back(texture);
}

// Return terrain size.
float Terrain::getTerrainSize() {
return m_terrainSize;
}

// Get a texture based on a given index.
Texture Terrain::getTexture(int i) {
    return m_textures[i];
}

// Return the size of the structure holding terrain textures.
int Terrain::getTextureVectorSize(){
    return m_textures.size();
}

// Set the light direction.
void Terrain::SetLightDirection(const glm::vec3& dir) {
    glm::vec3 reversedLightDir = dir * -1.0f;
    reversedLightDir = normalize(reversedLightDir);
    glUniform3f(m_reversedLightDirLoc, reversedLightDir.x, reversedLightDir.y, reversedLightDir.z);
}

// Set the light uniform variable.
void Terrain::SetLightUniform(GLuint loc) {
    m_reversedLightDirLoc = loc;
}

// Return the light direction.
glm::vec3 Terrain::getLightDirection() {
    return m_lightDirection;
}

// Find the center point of the terrain. (Used to center the skybox around the center of the terrain.)
void Terrain::findCenterOfTerrain() {
    float minX = std::numeric_limits<float>::max();
    float minY = std::numeric_limits<float>::max();
    float minZ = std::numeric_limits<float>::max();
    float maxX = std::numeric_limits<float>::min();
    float maxY = std::numeric_limits<float>::min();
    float maxZ = std::numeric_limits<float>::min();

    // Iterate through the vertexes to find the min/max of x, y, z values.
    for (const auto& vertex : m_vertices) {
        minX = std::min(minX, vertex.x);
        minY = std::min(minY, vertex.y);
        minZ = std::min(minZ, vertex.z);
        maxX = std::max(maxX, vertex.x);
        maxY = std::max(maxY, vertex.y);
        maxZ = std::max(maxZ, vertex.z);
    }

    // Calculate the center point
    float centerX = (minX + maxX) / 2.0f;
    float centerY = (minY + maxY) / 2.0f;
    float centerZ = (minZ + maxZ) / 2.0f;

    glm::vec3 center = {centerX, centerY, centerZ};
    m_centerOfTerrain = center;
 }

// Return the center location of the terrain.
 glm::vec3 Terrain::getCenterOfTerrain() {
    return m_centerOfTerrain;
 }

// Combination of pre-draw and draw. Sends uniforms to shaders.
void Terrain::renderTerrain() {
    glEnable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    // Set the polygon fill mode.
    glPolygonMode(GL_FRONT_AND_BACK, g.gPolygonMode);

    // Initialize clear color -- background of screen.
    glViewport(0,0,g.gScreenWidth,g.gScreenHeight);
    glClearColor(0.0f, 0.0f, 0.0f, 1.f);

    // Use our shader.
    glUseProgram(g.gGraphicsPipelineShaderProgram);

    // Light calculations.
    static float angle = 0.0f;
    angle += 0.001f; // Increase the angle over time to simulate movement

    // Calculate the x, y, z coordinates of the sun based on the angle
    float x = cos(angle);
    float y = sin(angle);
    float z = 0.5f; // Adjust the height of the light above the terrain.

    // Normalize the vector to ensure it has unit length.
    glm::vec3 lightDir = glm::normalize(glm::vec3(x, y, z));

    // Set the light direction.
    g.gTerrain.SetLightDirection(lightDir);

    // Send View Matrix uniform to shader.
    glm::mat4 ViewMatrix = g.gCamera.GetViewMatrix();
    GLint u_ViewMatrixLocation = glGetUniformLocation(g.gGraphicsPipelineShaderProgram, "u_ViewMatrix");
    if(u_ViewMatrixLocation>=0) {
        glUniformMatrix4fv(u_ViewMatrixLocation, 1, GL_FALSE, &ViewMatrix[0][0]);
    } else {
        std::cout << "Could not find u_ViewMatrix, maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }

    // Send Projection Matrix uniform to shader.
    glm::mat4 projection = glm::perspective(glm::radians(75.0f),
                                            (float)g.gScreenWidth/(float)g.gScreenHeight,
                                            0.1f,
                                            100000000.0f);
    GLint u_ProjectionLocation = glGetUniformLocation(g.gGraphicsPipelineShaderProgram, "u_Projection");
    if (u_ProjectionLocation >= 0) {
        glUniformMatrix4fv(u_ProjectionLocation, 1, GL_FALSE, &projection[0][0]);
    } else {
        std::cout << "Could not find u_projection, maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }


    // Send reversed light direction to shader.
    GLint u_ReversedLightDirLocation = glGetUniformLocation(g.gGraphicsPipelineShaderProgram, "u_ReversedLightDir");
    if (u_ReversedLightDirLocation >= 0) {
        glUniform3f(u_ReversedLightDirLocation, lightDir.x, lightDir.y, lightDir.z);
    } else {
        std::cout << "Could not find u_ReversedLightDir, maybe a mispelling?\n";
        exit(EXIT_FAILURE);
    }

    // Send water texture to shader.
    Texture water = g.gTerrain.getTexture(0);
    water.Bind(0);
    GLint u_water = glGetUniformLocation(g.gGraphicsPipelineShaderProgram, "waterTexture");
    if (u_water >= 0) {
        glUniform1i(u_water, 0);
    } else {
        std::cout << "Could not find waterTexture, maybe a misspelling?" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Send grass texture to shader.
    Texture grass = g.gTerrain.getTexture(1);
    grass.Bind(1);
    GLint u_grass = glGetUniformLocation(g.gGraphicsPipelineShaderProgram, "grassTexture");
    if (u_grass >= 0) {
        glUniform1i(u_grass, 1);
    } else {
        std::cout << "Could not find grassTexture, maybe a misspelling?" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Send rock texture to shader.
    Texture rock = g.gTerrain.getTexture(2);
    rock.Bind(2);
    GLint u_rock = glGetUniformLocation(g.gGraphicsPipelineShaderProgram, "rockTexture");
    if (u_rock >= 0) {
        glUniform1i(u_rock, 2);
    } else {
        std::cout << "Could not find rockTexture, maybe a misspelling?" << std::endl;
        exit(EXIT_FAILURE);
    }

    // Send snow texture to shader.
    Texture snow = g.gTerrain.getTexture(3);
    snow.Bind(3);
    GLint u_snow = glGetUniformLocation(g.gGraphicsPipelineShaderProgram, "snowTexture");
    if (u_snow >= 0) {
        glUniform1i(u_snow, 3);
    } else {
        std::cout << "Could not find snowTexture, maybe a misspelling?" << std::endl;
        exit(EXIT_FAILURE);
    }

    // -- Beginning of Draw --

    if (!g.drawObject) {
        return;
    }
    
    glBindVertexArray(g.gPlane_VAO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g.gPlane_IBO);

    glDrawElements(GL_TRIANGLES, (g.gTerrain.getDepth() - 1) * (g.gTerrain.getWidth() - 1) * 6, GL_UNSIGNED_INT, NULL);

    glUseProgram(0);
}

// Get the world height.
float Terrain::getWorldHeight(float x, float z) {
    float HeightMapX = x / m_worldScale;
    float HeightMapZ = z / m_worldScale;

    return getHeightInterpolated(HeightMapX, HeightMapZ);
}

// Performs bilinear interpolation to find the height at a specific location. (used to hold camera to terrain)
float Terrain::getHeightInterpolated(float x, float z) {
    // Sample the height at the bottom-left corner of the current cell.
    float BottomLeft = getVertexHeight((int)x, (int)z);

    // Check if the current cell is at the border of the terrain
    if (((int)x + 1 >= m_terrainSize) || ((int)z + 1 >= m_terrainSize)) {
        return BottomLeft;
    }

    // Sample the height at the bottom-right corner of the current cell
    float BottomRight = getVertexHeight((int)x + 1, (int)z);
    // Sample the height at the top-left corner of the current cell
    float TopLeft = getVertexHeight((int)x, (int)z + 1);
    // Sample the height at the top-right corner of the current cell
    float TopRight = getVertexHeight((int)x + 1, (int)z + 1);

    // Calculate the fractional part of the x-coordinate within the cell
    float factorX = x - floorf(x);

    // Interpolate the height between the bottom-left and bottom-right corners based on FactorX
    float InterpolatedBottom = (BottomRight - BottomLeft) * factorX + BottomLeft;
    // Interpolate the height between the top-left and top-right corners based on FactorX
    float InterpolatedTop = (TopRight - TopLeft) * factorX + TopLeft;

    // Calculate the fractional part of the z-coordinate within the cell
    float factorZ = z - floorf(z);

    // Interpolate the final height between the interpolated bottom and top heights based on factorZ
    float finalHeight = (InterpolatedTop - InterpolatedBottom) * factorZ + InterpolatedBottom;

    return finalHeight;
}

// Holds the camera position to the terrain.
void Terrain::ConstrainCameraToTerrain() {
    glm::vec3 newCameraPos = ConstrainCameraPosToTerrain(g.gCamera.GetCameraEyePosition());

    g.gCamera.SetCameraEyePosition(newCameraPos.x, newCameraPos.y, newCameraPos.z);

    float x = g.gCamera.GetCameraEyePosition().x;
    float y = g.gCamera.GetCameraEyePosition().y;
    float z = g.gCamera.GetCameraEyePosition().z;
}

// Sets the new camera position after every movement.
glm::vec3 Terrain::ConstrainCameraPosToTerrain(glm::vec3 cameraPos) {
    glm::vec3 newCameraPos = cameraPos;

    // Prevent Camera from moving off of terrain.
    if (cameraPos.x < 0.0f) {
        newCameraPos.x = 0.0f;
    }
    if (cameraPos.z < 0.0f) {
        newCameraPos.z = 0.0f;
    }
    if (cameraPos.x >= getWorldSize()) {
        newCameraPos.x = getWorldSize() - 0.5f;
    }
    if (cameraPos.z >= getWorldSize()) {
        newCameraPos.z = getWorldSize() - 0.5f;
    }

    // Set new camera height to height of the cell the camera is on, but raise it to simulate human height.
    newCameraPos.y = getWorldHeight(cameraPos.x, cameraPos.z) + m_cameraHeight;

    return newCameraPos;
}


// Return the size of the world.
float Terrain::getWorldSize() {
    return m_terrainSize * m_worldScale;
}

// Returns the height of a vertex given x and z coordinates.
float Terrain::getVertexHeight(int x, int z) {
    for (auto& vertex : m_vertices) {
        if ((vertex.x == x) && (vertex.z == z)) {
            return vertex.y;
        }
    }
    return 0.0f;
}