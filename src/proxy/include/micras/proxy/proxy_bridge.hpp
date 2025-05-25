#ifndef MICRAS_PROXY_BRIDGE_HPP
#define MICRAS_PROXY_BRIDGE_HPP

#include "micras/micras.hpp"
#include "micras/interface.hpp"
#include "micras/proxy/button.hpp"
#include "micras/proxy/buzzer.hpp"
#include "micras/proxy/fan.hpp"
#include "micras/proxy/imu.hpp"
#include "micras/proxy/rotary_sensor.hpp"
#include "micras/proxy/wall_sensors.hpp"
#include "micras/proxy/locomotion.hpp"
#include "micras/proxy/argb.hpp"
#include "micras/proxy/dip_switch.hpp"
#include "micras/proxy/stopwatch.hpp"
#include "micras/core/types.hpp"
#include "micrasverse_core/types.hpp"
#include "physics/box2d_micrasbody.hpp"
#include "micras/nav/state.hpp"
#include "micras/nav/actions/base.hpp"
#include <limits>
#include <algorithm>
#include <unordered_map>
#include <vector>
#include "constants.hpp"

namespace micras {

/**
 * @brief Class that provides access to Micras controller proxies.
 * This class is a friend of the Micras class and can access its private members.
 */
class ProxyBridge {
public:
    /**
     * @brief Construct a new ProxyBridge object.
     *
     * @param micras Reference to the Micras controller.
     * @param micrasBody Reference to the Box2D Micras body.
     */
    explicit ProxyBridge(Micras& micras, micrasverse::physics::Box2DMicrasBody& micrasBody);

    // Button access
    proxy::Button::Status   get_button_status() const;
    bool                    is_button_pressed() const;
    void                    set_button_status(proxy::Button::Status status);
    proxy::Button::PullType get_button_pull_type() const;
    void                    set_button_pull_type(proxy::Button::PullType pull_type);

    // Buzzer access
    void set_buzzer_frequency(float frequency);
    void set_buzzer_duration(uint32_t duration);
    void stop_buzzer();
    bool is_buzzer_playing() const;

    // Fan access
    void  set_fan_speed(float speed);
    float get_fan_speed() const;
    void  enable_fan();
    void  disable_fan();
    void  stop_fan();

    // IMU access
    void  update_imu();
    float get_imu_acceleration_x() const;
    float get_imu_acceleration_y() const;
    float get_imu_acceleration_z() const;
    float get_imu_angular_velocity_x() const;
    float get_imu_angular_velocity_y() const;
    float get_imu_angular_velocity_z() const;
    float get_imu_angular_velocity(proxy::Imu::Axis axis) const;
    float get_imu_linear_acceleration(proxy::Imu::Axis axis) const;
    void  calibrate_imu();

    // Rotary sensors access
    void  update_rotary_sensors();
    float get_left_rotary_sensor_position() const;
    float get_right_rotary_sensor_position() const;

    // Wall sensors access
    void  update_wall_sensors();
    float get_wall_sensor_distance(uint8_t index) const;
    bool  is_wall_detected(uint8_t index) const;
    float get_wall_sensor_reading(uint8_t sensor_index) const;
    float get_wall_sensor_adc_reading(uint8_t sensor_index) const;
    float get_wall_sensor_error(uint8_t sensor_index) const;
    void  calibrate_front_wall();
    void  calibrate_left_wall();
    void  calibrate_right_wall();
    void  turn_on_wall_sensors();
    void  turn_off_wall_sensors();

    // Locomotion access
    void set_wheel_command(float left_command, float right_command);
    void set_command(float linear, float angular);
    void stop_motors();
    void enable_motors();
    void disable_motors();

    // ARGB access
    void set_led_color(uint8_t index, uint8_t red, uint8_t green, uint8_t blue);
    void set_led_brightness(uint8_t index, uint8_t brightness);
    void set_argb_color(const micrasverse::types::Color& color, uint8_t index);
    void set_argb_color(const micrasverse::types::Color& color);
    void turn_off_argb(uint8_t index);
    void turn_off_argb();

    // Dip switch access
    bool              get_dip_switch_state(uint8_t index) const;
    std::vector<bool> get_dip_switch_states() const;
    void              set_dip_switch_state(uint8_t index, bool state);
    uint8_t           get_dip_switch_value() const;

    // Stopwatch access
    void     reset_stopwatch();
    uint32_t get_elapsed_time_ms() const;
    uint32_t get_elapsed_time_us() const;

    // Get the number of ARGB LEDs available
    size_t get_argb_count() const {
        return micras.argb ? 2 : 0;  // TArgb<2> has 2 LEDs
    }

    // MicrasController access
    micras::core::Objective              get_objective() const;
    std::shared_ptr<micras::nav::Action> get_current_action() const;
    std::string                          get_objective_string() const;
    std::string                          get_action_type_string() const;

    // Speed controller access
    micras::nav::SpeedController& get_speed_controller() const;

    float get_linear_pid_setpoint() const { return micras.speed_controller.linear_pid.setpoint; }

    float get_angular_pid_setpoint() const { return micras.speed_controller.angular_pid.setpoint; }

    micras::core::PidController& get_linear_pid() const { return micras.speed_controller.linear_pid; }

    micras::core::PidController& get_angular_pid() const { return micras.speed_controller.angular_pid; }

    float get_linear_pid_last_response() const { return micras.speed_controller.linear_pid.last_response; }

    float get_angular_pid_last_response() const { return micras.speed_controller.angular_pid.last_response; }

    float get_left_feed_forward_response() const { return micras.left_ff; }

    float get_right_feed_forward_response() const { return micras.right_ff; }

    float get_linear_speed() const { return micras.odometry.get_state().velocity.linear; }

    float get_angular_speed() const { return micras.odometry.get_state().velocity.angular; }

    float get_linear_pid_error_acc() const { return micras.speed_controller.linear_pid.error_acc; }

    float get_angular_pid_error_acc() const { return micras.speed_controller.angular_pid.error_acc; }

    float get_linear_pid_kp() const { return micras.speed_controller.linear_pid.kp; }

    float get_angular_pid_kp() const { return micras.speed_controller.angular_pid.kp; }

    float get_linear_pid_ki() const { return micras.speed_controller.linear_pid.ki; }

    float get_angular_pid_ki() const { return micras.speed_controller.angular_pid.ki; }

    float get_linear_pid_kd() const { return micras.speed_controller.linear_pid.kd; }

    float get_angular_pid_kd() const { return micras.speed_controller.angular_pid.kd; }

    float get_linear_integrative_response() const { return get_linear_pid_error_acc() * get_linear_pid_ki() * get_linear_pid_kp(); }

    float get_angular_integrative_response() const { return get_angular_pid_error_acc() * get_angular_pid_ki() * get_angular_pid_kp(); }

    float get_total_time() const { return micras.action_queuer.get_total_time(); }

    void reset_micras() { micras.reset(); }

    std::string get_follow_wall_type_string() const;

    micras::core::Vector get_current_goal() const;
    micras::nav::Pose    get_current_pose() const;

    bool has_wall(const micras::nav::GridPose& pose, bool consider_virtual = false) const {
        return micras.maze.costmap.has_wall(pose, consider_virtual);
    }

    micras::nav::Costmap<16, 16, 2>::WallState get_wall_state(const micras::nav::GridPose& pose) const {
        return micras.maze.costmap.get_cell(pose.position).walls.at(pose.orientation);
    }

    micras::nav::Costmap<16, 16, 2>::Cell get_cell(const micras::nav::GridPoint& point) const { return micras.maze.costmap.get_cell(point); }

    int get_maze_width() const { return micras::maze_width; }

    int get_maze_height() const { return micras::maze_height; }

    std::vector<int16_t> get_maze_cell_costs() const {
        const int            width = get_maze_width();
        const int            height = get_maze_height();
        std::vector<int16_t> costs(width * height);

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int16_t cost = micras.maze.costmap.get_cost(micras::nav::GridPoint(x, y), micras::nav::Maze::Layer::EXPLORE) +
                               micras.maze.costmap.get_cost(micras::nav::GridPoint(x, y), micras::nav::Maze::Layer::RETURN);
                costs[y * width + x] = cost > 125 ? 125 : cost;
            }
        }

        return costs;
    }

    core::Vector getOffset() const {
        micras::nav::Pose pose;
        pose.position = {
            micrasBody.getPosition().x - micrasverse::WALL_THICKNESS / 2.0f, micrasBody.getPosition().y - micrasverse::WALL_THICKNESS / 2.0f
        };
        pose.orientation = micrasBody.getAngle() + B2_PI / 2.0f;
        core::Vector offset = pose.to_cell(micras::cell_size);
        return offset;
    }

    std::unordered_map<micras::nav::GridPose, micras::nav::MazeGraph::Node> get_maze_graph() const { return micras.maze.graph.graph; }

    uint8_t get_min_maze_cost() const { return micras.maze.minimum_cost; }

    const std::list<micras::nav::GridPose>& get_best_route() const { return micras.maze.get_best_route(); }

    void send_event(Interface::Event event);
    bool acknowledge_event(Interface::Event event);
    bool peek_event(Interface::Event event) const;

private:
    Micras&                                micras;
    micrasverse::physics::Box2DMicrasBody& micrasBody;
};

}  // namespace micras

#endif  // MICRAS_PROXY_BRIDGE_HPP
