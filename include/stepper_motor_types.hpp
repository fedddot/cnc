#ifndef	STEPPER_MOTOR_TYPES_HPP
#define	STEPPER_MOTOR_TYPES_HPP

#include <map>
#include <vector>

#include "gpio.hpp"

namespace cnc {
	enum class Shoulder: int { IN1, IN2, IN3, IN4 };
	using GpoId = int;
	using Shoulders = std::map<Shoulder, GpoId>;
	using GpioState = typename mcu_platform::Gpio::State;
	using MotorState = std::map<Shoulder, GpioState>;
	using MotorStates = std::vector<MotorState>;
}

#endif // STEPPER_MOTOR_TYPES_HPP