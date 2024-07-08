#pragma once

#include "glm/glm.hpp"

class Camera {
    public:
        Camera();
        // Return a view matrix without camera transformation applied.
        glm::mat4 GetViewMatrix() const;

        // Move Camera around.
        void MouseLook(int mouseX, int mouseY);
        void MoveForward(float speed);
        void MoveBackward(float speed);
        void MoveLeft(float speed);
        void MoveRight(float speed);
        void MoveUp(float speed);
        void MoveDown(float speed);
        void SetCameraEyePosition(float x, float y, float z);
        glm::vec3 GetCameraEyePosition();

        // Setters and getters.
        float GetEyeXPosition();
        float GetEyeYPosition();
        float GetEyeZPosition();
        float GetViewXDirection();
        float GetViewYDirection();
        float GetViewZDirection();

    private:
        glm::vec2 m_oldMousePosition;
        glm::vec3 m_eyePosition;
        glm::vec3 m_viewDirection;
        glm::vec3 m_upVector;
        glm::vec3 m_rightVector;
        glm::mat4 m_yawMatrix;
};