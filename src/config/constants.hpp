#ifndef CONSTANTS_HPP
#define CONSTANTS_HPP

#include "box2d/box2d.h"

namespace micrasverse {

    constexpr float STEP = 1.0f / 60.0f;                                                    // s — simulation step time

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
    constexpr float MICRAS_RESTITUTION = 0.0f;                                              // bounciness
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

}  // namespace micrasverse

#include <cstdint>
#include <numbers>

#include "micras/core/pid_controller.hpp"
#include "micras/nav/go_to_point.hpp"
#include "micras/nav/grid_pose.hpp"
#include "micras/nav/look_at_point.hpp"
#include "micras/nav/mapping.hpp"
#include "micras/nav/odometry.hpp"
#include "micras/proxy/storage.hpp"

namespace micras {
/*****************************************
 * Constants
 *****************************************/

constexpr uint8_t  maze_width{16};
constexpr uint8_t  maze_height{16};
constexpr float    cell_size{0.18};
constexpr uint32_t loop_time_us{1042};

/*****************************************
 * Template Instantiations
 *****************************************/

namespace nav {
using Maze = TMaze<maze_width, maze_height>;
using Mapping = TMapping<maze_width, maze_height>;
}  // namespace nav

/*****************************************
 * Configurations
 *****************************************/

const nav::LookAtPoint::Config look_at_point_config{
    .linear_pid =
        {
            .kp = 50.0F,
            .ki = 0.0F,
            .kd = 0.0F,
            .setpoint = 0.0F,
            .saturation = 20.0F,
            .max_integral = -1.0F,
        },
    .angular_pid =
        {
            .kp = 150.0F,
            .ki = 0.5F,
            .kd = 0.01F,
            .setpoint = 0.0F,
            .saturation = 20.0F,
            .max_integral = 15.0F,
        },
    .distance_tolerance = 0.015F,
    .velocity_tolerance = 0.015F,
};

const nav::GoToPoint::Config go_to_point_config{
    .stop_pid =
        {
            .kp = 270.0F,
            .ki = 0.5F,
            .kd = 0.08F,
            .setpoint = 0.0F,
            .saturation = 7.0F,
            .max_integral = 30.0F,
        },
    .angular_pid =
        {
            .kp = 150.0F,
            .ki = 0.0F,
            .kd = 0.02F,
            .setpoint = 0.0F,
            .saturation = 60.0F,
            .max_integral = -1.0F,
        },
    .cell_size = cell_size,
    .min_linear_command = 5.0F,
    .max_linear_command = 15.0F,
    .deceleration_factor = 0.3F,
    .distance_tolerance = 0.025F,
    .velocity_tolerance = 0.02F,
};

const nav::FollowWall::Config follow_wall_config = {
    .pid =
        {
            .kp = 100.0F,
            .ki = 0.0F,
            .kd = 0.04F,
            .setpoint = 0.0F,
            .saturation = 200.0F,
            .max_integral = -1.0F,
        },
    .base_left_reading = 0.168,
    .base_right_reading = 0.163F,
};

const nav::Mapping::Config mapping_config{
    .wall_thickness = 0.0126,
    .cell_size = cell_size,
    .front_sensor_pose = {{0.028F, 0.045F}, 0.0F},
    .side_sensor_pose = {{0.009F, 0.055F}, std::numbers::pi_v<float> / 6.0F},
    .front_distance_alignment_tolerance = 0.04F,
    .side_distance_alignment_tolerance = 0.02F,
    .front_orientation_alignment_tolerance = 0.02F,
    .side_orientation_alignment_tolerance = 0.02F,
    .front_distance_reading = {{
        0.57F,
        0.60F,
    }},
    .front_orientation_reading = {{
        0.32F,
        0.37F,
    }},
    .side_distance_reading = {{
        0.17F,
        0.12F,
    }},
    .start = {{0, 0}, nav::Side::UP},
};

const nav::Odometry::Config odometry_config{
    .linear_cutoff_frequency = 5.0F,
    .wheel_radius = 0.0112F,
    .initial_pose =
        {
            {0.09F, 0.04F + mapping_config.wall_thickness / 2.0F},
            static_cast<uint8_t>(mapping_config.start.orientation) * std::numbers::pi_v<float> / 2,
        },
};
}  // namespace micras


#endif // CONSTANTS_HPP
