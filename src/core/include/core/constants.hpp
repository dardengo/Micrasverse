#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include "box2d/box2d.h"

namespace micrasverse {

    constexpr b2Vec2 GRAVITY = {0.0f, +0.0f};                                               // m/s^2 — set to {0.0f} for top-down view
    
    constexpr int MAZE_CELLS_HEIGHT = 16;                                                   // number of cells
    constexpr int MAZE_CELLS_WIDTH = 16;                                                    // number of cells
    
    constexpr float CELL_SIZE = 0.18f;                                                      // m
    constexpr float WALL_THICKNESS = 0.012f;                                                // m
    constexpr float WALL_SIZE = CELL_SIZE - WALL_THICKNESS;                                 // m
    
    constexpr float MAZE_FLOOR_WIDTH = CELL_SIZE * MAZE_CELLS_WIDTH + WALL_THICKNESS;       // m
    constexpr float MAZE_FLOOR_HALFWIDTH = MAZE_FLOOR_WIDTH / 2.0f;                         // m
    constexpr float MAZE_FLOOR_HEIGHT = CELL_SIZE * MAZE_CELLS_HEIGHT + WALL_THICKNESS;     // m
    constexpr float MAZE_FLOOR_HALFHEIGHT = MAZE_FLOOR_HEIGHT / 2.0f;                       // m


    constexpr float MICRAS_WIDTH = 0.067f;                                                  // m
    constexpr float MICRAS_HALFWIDTH = MICRAS_WIDTH / 2.0f;                                 // m
    constexpr float MICRAS_HEIGHT = 0.10f;                                                  // m
    constexpr float MICRAS_HALFHEIGHT = MICRAS_HEIGHT / 2.0f;                               // m
    constexpr float MICRAS_MASS = 0.110f;                                                   // kg
    constexpr float MICRAS_RESTITUTION = 0.3f;                                              // bounciness
    constexpr float MICRAS_FRICTION = 0.95f;                                                // friction coefficient
    constexpr float MICRAS_WHEEL_RADIUS = 0.0112f;                                          // m
    constexpr float MICRAS_GEAR_RATIO = 2.5f;                                               // reduction ratio
    constexpr float MICRAS_TRACK_WIDTH = 0.067f;                                            // m

    constexpr float MOTOR_MAX_VOLTAGE = 12.0f;                                              // V
    constexpr float MOTOR_STALL_TORQUE = 0.0008f * 9.81f;                                   // Nm
    constexpr float MOTOR_STALL_CURRENT = 2.0f;                                             // A
    constexpr float MOTOR_MAX_RPM = 17000.0f;                                               // rpm
    constexpr float MOTOR_MAX_ANGULAR_VELOCITY = MOTOR_MAX_RPM * 2.0f * B2_PI / 60.0f;      // rad/s
    constexpr float MOTOR_RESISTANCE = 6.0f;                                                // Ohms
    constexpr float MOTOR_KE = MOTOR_MAX_VOLTAGE / MOTOR_MAX_ANGULAR_VELOCITY;              // V/(rad/s)
    constexpr float MOTOR_KT = MOTOR_STALL_TORQUE / MOTOR_STALL_CURRENT;                    // Nm/A

}

#endif // CONSTANTS_HPP
