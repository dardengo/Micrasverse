#ifndef CAMERA_H
#define CAMERA_H

#include "core/constants.h"

#define GLFW_INCLUDE_NONE   // GLFW include guard
#include "glad/glad.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"

namespace micrasverse::render{

//Enum to represent directions for movement

enum class CameraDirection {
    NONE = 0,
    FORWARD,
    BACKWARD,
    LEFT,
    RIGHT,
    UP,
    DOWN
};

// Camera class to help display from POV of camera
class Camera {
public:
    // Camera config

    // Position
    glm::vec3 position;

    // Directional values
    glm::vec3 cameraFront;
    glm::vec3 cameraUp;
    glm::vec3 cameraRight;
    glm::vec3 worldUp;

    // Rotational values
    float yaw; // x-axis
    float pitch; // y-axis

    // Movement values
    float speed;
    float sensitivity;
    float zoom;

    // Default and initialize with position
    Camera(const glm::vec3 position = glm::vec3(MAZE_FLOOR_HALFWIDTH, MAZE_FLOOR_HALFHEIGHT, 3.75f));

    // Modifiers

    // Change camera direction (mouse movement)
    void updateCameraDirection(const double dx, const double dy);

    // Change camera position in certain direction (keyboard)
    void updateCameraPos(const CameraDirection direction, const double dt);

    // Change camera zoom (scroll wheel)
    void updateCameraZoom(const double dy);

    // Accessors

    // Get view matrix for camera
    glm::mat4 getViewMatrix();

    // Set zoom value for camera
    void setZoom(const float zoom);

    // Get zoom value for camera
    float getZoom();

    void processInput(const double dt);

private:
    // Private modifier

    // Change camera directional vectors based on movement
    void updateCameraVectors();
};

} // namespace micrasverse::render

#endif // CAMERA_H
