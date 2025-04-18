#ifndef MICRAS_TARGET_HPP
#define MICRAS_TARGET_HPP

#include "micras/proxy/argb.hpp"
#include "micras/proxy/battery.hpp"
#include "micras/proxy/button.hpp"
#include "micras/proxy/buzzer.hpp"
#include "micras/proxy/dip_switch.hpp"
#include "micras/proxy/fan.hpp"
#include "micras/proxy/imu.hpp"
#include "micras/proxy/led.hpp"
#include "micras/proxy/locomotion.hpp"
#include "micras/proxy/rotary_sensor.hpp"
#include "micras/proxy/stopwatch.hpp"
#include "micras/proxy/storage.hpp"
#include "micras/proxy/torque_sensors.hpp"
#include "micras/proxy/wall_sensors.hpp"

namespace micras {
/*****************************************
 * Template Instantiations
 *****************************************/

namespace proxy {
using Argb = proxy::TArgb<2>;
using DipSwitch = TDipSwitch<4>;
using TorqueSensors = TTorqueSensors<2>;
using WallSensors = TWallSensors<4>;
}  // namespace proxy

}  // namespace micras

#endif  //  MICRAS_TARGET_HPP
