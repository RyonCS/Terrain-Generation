#pragma once
#include <vector>
#include "glm/glm.hpp"
#include "Texture.hpp"
#include "HeightMap.hpp"

struct Vertex {
    float x, y, z;
    float u, v;
    float normal_x, normal_y, normal_z;
};

class Terrain {
    public:
        Terrain();

        ~Terrain();

        void Initialize();

        int getDepth();

        int getWidth();

        void setHeightMap(HeightMap heightMap);

        HeightMap getHeightMap();

        void generatePlane();

        std::vector<Vertex>& getVertices();

        std::vector<unsigned int>& getTriangleIndices();

        void InitIndices(std::vector<unsigned int>& indices);

        void addTexture(Texture& texture);

        float getTerrainSize();

        float GetWorldScale() {
            return m_worldScale;
        }

        Texture getTexture(int i);

        int getTextureVectorSize();

        void setTexturePath(std::string texturePath);

        std::string getTexturePath();

        void CalculateNormals();

        void SetLightDirection(const glm::vec3& dir);

        void SetLightUniform(GLuint loc);

        glm::vec3 getLightDirection();

        void findCenterOfTerrain();

        glm::vec3 getCenterOfTerrain();

        void renderTerrain();

        float getWorldSize();

        float getWorldHeight(float x, float z);

        float getHeightInterpolated(float x, float z);
        
        void ConstrainCameraToTerrain();

        glm::vec3 ConstrainCameraPosToTerrain(glm::vec3 cameraPos);

        float getVertexHeight(int x, int z);
        
    private:
        std::string heightMapFile;
        HeightMap m_heightMap;
        glm::vec3 m_centerOfTerrain;

        int m_width = 0;
        int m_depth = 0;
        int m_terrainSize = 0;

        std::vector<Vertex> m_vertices;
        std::vector<unsigned int> m_triangleIndices;

        std::vector<Texture> m_textures;
        std::string waterTexturePath = "./thirdparty/resources/Terrain/water.ppm";
        std::string grassTexturePath = "./thirdparty/resources/Terrain/grass.ppm";
        std::string rockTexturePath = "./thirdparty/resources/Terrain/rock.ppm";
        std::string snowTexturePath = "./thirdparty/resources/Terrain/snow.ppm";

        float m_worldScale = 1.0f;
        glm::vec3 m_lightDirection;
        GLuint m_reversedLightDirLoc = -1;
        float m_cameraHeight = 3.0f;
};

