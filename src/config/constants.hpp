#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include "box2d/box2d.h"
#include <string_view>

namespace micrasverse {

// Maze dimensions (1 unit = 1 meter)
constexpr int MAZE_CELLS_HEIGHT = 16;  // number of cells
constexpr int MAZE_CELLS_WIDTH = 16;   // number of cells

constexpr float CELL_SIZE = 0.18f;                       // meters
constexpr float WALL_THICKNESS = 0.012f;                 // meters
constexpr float WALL_SIZE = CELL_SIZE - WALL_THICKNESS;  // meters

constexpr float MAZE_FLOOR_WIDTH = CELL_SIZE * MAZE_CELLS_WIDTH + WALL_THICKNESS;    // meters
constexpr float MAZE_FLOOR_HALFWIDTH = MAZE_FLOOR_WIDTH / 2.0f;                      // meters
constexpr float MAZE_FLOOR_HEIGHT = CELL_SIZE * MAZE_CELLS_HEIGHT + WALL_THICKNESS;  // meters
constexpr float MAZE_FLOOR_HALFHEIGHT = MAZE_FLOOR_HEIGHT / 2.0f;                    // meters

// Micras robot physical dimensions and properties
constexpr float MICRAS_WIDTH = 0.067f;                                     // meters
constexpr float MICRAS_HALFWIDTH = MICRAS_WIDTH / 2.0f;                    // meters
constexpr float MICRAS_HEIGHT = 0.1f;                                      // meters
constexpr float MICRAS_HALFHEIGHT = MICRAS_HEIGHT / 2.0f;                  // meters
constexpr float MICRAS_MASS = 0.110f;                                      // kilograms
constexpr float MICRAS_RESTITUTION = 0.0f;                                 // bounciness (0-1)
constexpr float MICRAS_FRICTION = 0.95f;                                   // friction coefficient
constexpr float MICRAS_WHEEL_RADIUS = 0.0112f;                             // meters
constexpr float MICRAS_GEAR_RATIO = 2.5f;                                  // reduction ratio
constexpr float MICRAS_TRACK_WIDTH = 0.067f;                               // meters (distance between wheels)
constexpr float MICRAS_CENTER_OF_MASS_OFFSET = 0.04f - MICRAS_HALFHEIGHT;  // meters (offset from center of body)

// Motor characteristics
constexpr float MOTOR_MAX_VOLTAGE = 12.0f;                                          // volts
constexpr float MOTOR_STALL_TORQUE = 0.00063f * 9.81f;                              // newton-meters
constexpr float MOTOR_STALL_CURRENT = 2.0f;                                         // amperes
constexpr float MOTOR_MAX_RPM = 13000.0f;                                           // revolutions per minute
constexpr float MOTOR_MAX_ANGULAR_VELOCITY = MOTOR_MAX_RPM * 2.0f * B2_PI / 60.0f;  // radians/second
constexpr float MOTOR_RESISTANCE = MOTOR_MAX_VOLTAGE / MOTOR_STALL_CURRENT;         // ohms
constexpr float MOTOR_KE = MOTOR_MAX_VOLTAGE / MOTOR_MAX_ANGULAR_VELOCITY;          // volts/(radians/second)
constexpr float MOTOR_KT = MOTOR_STALL_TORQUE / MOTOR_STALL_CURRENT;                // newton-meters/ampere

// Simulation parameters
// constexpr std::string_view DEFAULT_MAZE_PATH = "external/mazefiles/training/minimaze.txt";
// constexpr std::string_view DEFAULT_MAZE_PATH = "external/mazefiles/classic/alljapan-015-1994-frsh.txt";
constexpr std::string_view DEFAULT_MAZE_PATH = "external/mazefiles/classic/br2024-robochallenge-day3.txt";
constexpr float            STEP = 1.0f / 1000.0f;   // seconds — simulation step time
constexpr b2Vec2           GRAVITY = {0.0f, 0.0f};  // m/s² — set to {0.0f} for top-down view

// Rendering parameters
constexpr int      WINDOW_WIDTH = 1280;       // pixels
constexpr int      WINDOW_HEIGHT = 720;       // pixels
constexpr uint32_t MAX_FRAMES_IN_FLIGHT = 2;  // number of frames in flight

}  // namespace micrasverse

#include <cstdint>
#include <numbers>

#include "micras/nav/action_queuer.hpp"
#include "micras/nav/follow_wall.hpp"
#include "micras/nav/maze.hpp"
#include "micras/nav/odometry.hpp"
#include "micras/nav/speed_controller.hpp"

namespace micras {
/***************
 * Constants
 ***************/

constexpr uint8_t  maze_width{16};
constexpr uint8_t  maze_height{16};
constexpr float    cell_size{0.18};
constexpr uint32_t loop_time_us = micrasverse::STEP * 1e6F;
constexpr float    wall_thickness{0.012F};
constexpr float    start_offset{0.05F + wall_thickness / 2.0F};
constexpr float    max_linear_acceleration{9.0F};
constexpr float    max_linear_deceleration{9.0F};
constexpr float    max_angular_acceleration{300.0F};
constexpr float    crash_acceleration{1000000.0F};
constexpr float    fan_speed{100.0F};

constexpr core::WallSensorsIndex wall_sensors_index{
    .left_front = 0,
    .left = 1,
    .right = 2,
    .right_front = 3,
};

/***************
 * Template Instantiations
 ***************/

namespace nav {
using Maze = TMaze<maze_width, maze_height>;
}  // namespace nav

/***************
 * Configurations
 ***************/

const nav::ActionQueuer::Config action_queuer_config{
    .cell_size = cell_size,
    .start_offset = start_offset,
    .curve_safety_margin = 0.0375F + 0.015F,
    .exploring =
        {
            .max_linear_speed = 0.4F,
            .max_linear_acceleration = max_linear_acceleration,
            .max_linear_deceleration = max_linear_deceleration,
            .max_centrifugal_acceleration = 2.0F,
            .max_angular_acceleration = max_angular_acceleration,
        },
    .solving =
        {
            .max_linear_speed = 3.0F,
            .max_linear_acceleration = max_linear_acceleration,
            .max_linear_deceleration = max_linear_deceleration,
            .max_centrifugal_acceleration = 5.0F,
            .max_angular_acceleration = max_angular_acceleration,
        },
};

const nav::FollowWall::Config follow_wall_config{
    .pid =
        {
            .kp = 15.0F,
            .ki = 0.0F,
            .kd = 0.008F,
            .setpoint = 0.0F,
            .saturation = 1.0F,
            .max_integral = -1.0F,
        },
    .wall_sensor_index = wall_sensors_index,
    .max_linear_speed = 0.5F,
    .max_angular_acceleration = max_angular_acceleration,
    .cell_size = cell_size,
    .post_threshold = 4.0F,
    .post_reference = 0.44F * cell_size,
    .post_clearance = 0.035F,
};

const nav::Maze::Config maze_config{
    .start = {{0, 0}, nav::Side::UP},
    .goal = {{
        {maze_width / 2, maze_height / 2},
        {(maze_width - 1) / 2, maze_height / 2},
        {maze_width / 2, (maze_height - 1) / 2},
        {(maze_width - 1) / 2, (maze_height - 1) / 2},
    }},
    .cost_margin = 1.2F,
    .graph_config =
        {
            .cell_size = cell_size,
            .cost_params = action_queuer_config.solving,
        },
};

const nav::Odometry::Config odometry_config{
    .linear_cutoff_frequency = 50.0F,
    .wheel_radius = 0.0112F,
    .initial_pose = {{cell_size / 2.0f, start_offset}, std::numbers::pi_v<float> / 2.0f},
};

const nav::SpeedController::Config speed_controller_config{
    .linear_pid =
        {
            .kp = 10.0F,
            .ki = 10.0F,
            .kd = 0.0F,
            .setpoint = 0.0F,
            .saturation = 20.0F,
            .max_integral = -1.0F,
        },
    .angular_pid =
        {
            .kp = 0.7F,
            .ki = 5.0F,
            .kd = 0.0F,
            .setpoint = 0.0F,
            .saturation = 20.0F,
            .max_integral = -1.0F,
        },
    .left_feed_forward =
        {
            .linear_speed = 16.4F,
            .linear_acceleration = 3.98F,
            .angular_speed = -0.55F,
            .angular_acceleration = -0.143F,
        },
    .right_feed_forward =
        {
            .linear_speed = 16.4F,
            .linear_acceleration = 3.98F,
            .angular_speed = +0.55F,
            .angular_acceleration = +0.143F,
        },
};
}  // namespace micras

#endif  // MICRAS_CONSTANTS_HPP
