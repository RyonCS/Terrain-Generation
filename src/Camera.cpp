#include "Camera.hpp"

#include "glm/gtx/transform.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include <iostream>

// Handles camera based on mouse movement.
void Camera::MouseLook(int mouseX, int mouseY) {
    // Record our new position as a vector
    glm::vec2 newMousePosition(mouseX, mouseY);

    // Little hack for our mouse look function so we can move our camera.
    static bool firstLook=true;
    if(true == firstLook) {
        firstLook=false;
        m_oldMousePosition = newMousePosition;
    }

    // Calculate the amount of mouse movement.
    glm::vec2 mouseMovement = newMousePosition - m_oldMousePosition;

    // Calculate the yaw and pitch matrices.
    glm::mat4 yawMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(mouseMovement.x), m_upVector);
    glm::mat4 pitchMatrix = glm::rotate(glm::mat4(1.0f), glm::radians(mouseMovement.y), m_rightVector);

    // Calculate the new view direction based on the yaw and pitch matrices.
    m_viewDirection = glm::mat3(yawMatrix * pitchMatrix) * m_viewDirection;

    // Set old mouse position.
    m_oldMousePosition = newMousePosition;
}

// Move camera forward on W.
void Camera::MoveForward(float speed) {
    m_eyePosition += speed * m_viewDirection;
}

// Move camera backwards on S.
void Camera::MoveBackward(float speed){
    m_eyePosition -= speed * m_viewDirection;
}

// Move camera left on A.
void Camera::MoveLeft(float speed) {
    m_rightVector = glm::cross(m_viewDirection, m_upVector);
    m_eyePosition -= speed * m_rightVector;
}

// Move camera right on D.
void Camera::MoveRight(float speed){
    m_rightVector = glm::cross(m_viewDirection, m_upVector);
    m_eyePosition += speed * m_rightVector;
}

// Move camera up on UP.
void Camera::MoveUp(float speed) {
    m_eyePosition.y += speed;
}

// Move camera down on DOWN.
void Camera::MoveDown(float speed) {
    m_eyePosition.y -= speed;
}

// Sets the camera's current eye position.
void Camera::SetCameraEyePosition(float x, float y, float z) {
    m_eyePosition.x = x;
    m_eyePosition.y = y;
    m_eyePosition.z = z;
}

float Camera::GetEyeXPosition() {
    return m_eyePosition.x;
}

float Camera::GetEyeYPosition() {
    return m_eyePosition.y;
}

float Camera::GetEyeZPosition() {
    return m_eyePosition.z;
}

float Camera::GetViewXDirection() {
    return m_viewDirection.x;
}

float Camera::GetViewYDirection(){
    return m_viewDirection.y;
}

float Camera::GetViewZDirection() {
    return m_viewDirection.z;
}

Camera::Camera(){
    m_eyePosition = glm::vec3(40.9124f, 3.61214f, 295.917f);
    m_viewDirection = glm::vec3(3.2f, -2.6f, 4.11f);
    m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);
    m_rightVector = glm::cross(m_viewDirection, m_upVector);
}

glm::mat4 Camera::GetViewMatrix() const {
    return glm::lookAt(m_eyePosition, m_eyePosition + m_viewDirection, m_upVector);
}

glm::vec3 Camera::GetCameraEyePosition() {
    return m_eyePosition;
}