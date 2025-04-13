#include "render/camera.hpp"

namespace micrasverse::render {

Camera::Camera(const glm::vec3 position)
    : position(position),
    yaw(-90.0f),    // -90.0f to face x-y plane
    pitch(0.0f),
    speed(1.0f),
    sensitivity(1.0f),
    zoom(45.0f),
    cameraFront(glm::vec3(0.0f, 0.0f, -1.0f)),  // -z direction
    worldUp(glm::vec3(0.0f, 1.0f, 0.0f))
{
    updateCameraVectors();
}

void Camera::updateCameraDirection(const double dx, const double dy) {
    yaw += dx;
    pitch += dy;

    // Constrain pitch to prevent camera flipping vertically
    if (pitch > 89.0f) {
        pitch = 89.0f;
    }
    if (pitch < -89.0f) {
        pitch = -89.0f;
    }

    updateCameraVectors();
}

void Camera::updateCameraPos(const CameraDirection direction, const double dt) {
    const float velocity = (float)dt * speed;

    switch (direction) {
    case CameraDirection::FORWARD:
        position += cameraFront * velocity;
        break;
    case CameraDirection::BACKWARD:
        position -= cameraFront * velocity;
        break;
    case CameraDirection::RIGHT:
        position += cameraRight * velocity;
        break;
    case CameraDirection::LEFT:
        position -= cameraRight * velocity;
        break;
    case CameraDirection::UP:
        position += cameraUp * velocity;
        break;
    case CameraDirection::DOWN:
        position -= cameraUp * velocity;
        break;
    case CameraDirection::NONE:
        break;
    }
}

void Camera::updateCameraZoom(const double dy) {
    zoom -= 5 * dy; // Multiply by 5 to make zooming faster
    
    // Constrain zoom value
    if (zoom < 1.0f) {
        zoom = 1.0f;
    }
    else if (zoom > 100.0f) {
        zoom = 100.0f;
    }
}

glm::mat4 Camera::getViewMatrix() {
    return glm::lookAt(position, position + cameraFront, cameraUp);
}

void Camera::setZoom(const float zoom) {
    this->zoom = zoom;
}

float Camera::getZoom() {
    return zoom;
}

void Camera::updateCameraVectors() {
    glm::vec3 direction = {};
    direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    direction.y = sin(glm::radians(pitch));
    direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
    cameraFront = glm::normalize(direction);

    cameraRight = glm::normalize(glm::cross(cameraFront, worldUp));
    cameraUp = glm::normalize(glm::cross(cameraRight, cameraFront));
}

void Camera::update(const micrasverse::physics::Box2DMicrasBody& micrasBody) {
    // Update camera position to follow Micras
    if (this->followMicras) {
        this->position = {micrasBody.getPosition().x, micrasBody.getPosition().y, this->position.z};
    }
}

} // namespace micrasverse::render
