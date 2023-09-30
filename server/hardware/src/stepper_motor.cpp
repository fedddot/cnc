#include <algorithm>
#include <stdexcept>
#include <map>
#include <memory>
#include <vector>

#include "gpio.hpp"
#include "output_gpio.hpp"

#include "stepper_motor.hpp"

using namespace hardware;

const StepperMotor::ShouldersStates hardware::StepperMotor::m_states(StepperMotor::init_states());

StepperMotor::StepperMotor(const ControlPinLayout& pin_layout): m_gpios(init_gpios(pin_layout)), m_current_shoulders_state_iter(m_states.begin()) {

}

void StepperMotor::step(Direction direction) {
	auto next_state_iter = next_state(m_current_shoulders_state_iter, direction);
	apply_state(*next_state_iter, m_gpios);
	m_current_shoulders_state_iter = next_state_iter;
}

StepperMotor::ShouldersStates::const_iterator StepperMotor::next_state(ShouldersStates::const_iterator from, Direction direction) {
	if (m_states.end() == from) {
		throw std::invalid_argument("invalid from iterator received");
	}

	auto proceed = [&](ShouldersStates::const_iterator from) {
		auto next = from + 1;
		if (next == m_states.end()) {
			return m_states.begin();
		}
		return next;
	};

	auto preceed = [&](ShouldersStates::const_iterator from) {
		if (m_states.begin() == from) {
			return m_states.begin() + m_states.size() - 1;
		}
		auto prev = m_states.begin();
		while (prev + 1 != from) {
			++prev;
		}
		return prev;
	};

	switch (direction)
	{
	case Direction::CW:
		return proceed(from);
	case Direction::CCW:
		return preceed(from);
	default:
		break;
	}
	throw std::invalid_argument("unsupported direction received");
}

StepperMotor::ControlGpios StepperMotor::init_gpios(const ControlPinLayout& pin_layout) {
	ControlGpios result;
	std::for_each(
		pin_layout.begin(),
		pin_layout.end(),
		[&](const auto& iter) {
			result.insert({iter.first, std::shared_ptr<OutputGpio>(new OutputGpio(iter.second))});
		}
	);
	return result;
}

StepperMotor::ShouldersStates StepperMotor::init_states() {
	return ShouldersStates(
		{
			// 0 degrees
			{
				{BridgeShoulder::A_LEFT, Gpio::Value::HIGH},
				{BridgeShoulder::A_RIGHT, Gpio::Value::LOW},
				{BridgeShoulder::B_LEFT, Gpio::Value::LOW},
				{BridgeShoulder::B_RIGHT, Gpio::Value::LOW}
			},
			// 45 degrees
			{
				{BridgeShoulder::A_LEFT, Gpio::Value::HIGH},
				{BridgeShoulder::A_RIGHT, Gpio::Value::LOW},
				{BridgeShoulder::B_LEFT, Gpio::Value::HIGH},
				{BridgeShoulder::B_RIGHT, Gpio::Value::LOW}
			},
			// 90 degrees
			{
				{BridgeShoulder::A_LEFT, Gpio::Value::LOW},
				{BridgeShoulder::A_RIGHT, Gpio::Value::LOW},
				{BridgeShoulder::B_LEFT, Gpio::Value::HIGH},
				{BridgeShoulder::B_RIGHT, Gpio::Value::LOW}
			},
			// 135 degrees
			{
				{BridgeShoulder::A_LEFT, Gpio::Value::LOW},
				{BridgeShoulder::A_RIGHT, Gpio::Value::HIGH},
				{BridgeShoulder::B_LEFT, Gpio::Value::HIGH},
				{BridgeShoulder::B_RIGHT, Gpio::Value::LOW}
			},
			// 180 degrees
			{
				{BridgeShoulder::A_LEFT, Gpio::Value::LOW},
				{BridgeShoulder::A_RIGHT, Gpio::Value::HIGH},
				{BridgeShoulder::B_LEFT, Gpio::Value::LOW},
				{BridgeShoulder::B_RIGHT, Gpio::Value::LOW}
			},
			// 225 degrees
			{
				{BridgeShoulder::A_LEFT, Gpio::Value::LOW},
				{BridgeShoulder::A_RIGHT, Gpio::Value::HIGH},
				{BridgeShoulder::B_LEFT, Gpio::Value::LOW},
				{BridgeShoulder::B_RIGHT, Gpio::Value::HIGH}
			},
			// 270 degrees
			{
				{BridgeShoulder::A_LEFT, Gpio::Value::LOW},
				{BridgeShoulder::A_RIGHT, Gpio::Value::LOW},
				{BridgeShoulder::B_LEFT, Gpio::Value::LOW},
				{BridgeShoulder::B_RIGHT, Gpio::Value::HIGH}
			},
			// 315 degrees
			{
				{BridgeShoulder::A_LEFT, Gpio::Value::HIGH},
				{BridgeShoulder::A_RIGHT, Gpio::Value::LOW},
				{BridgeShoulder::B_LEFT, Gpio::Value::LOW},
				{BridgeShoulder::B_RIGHT, Gpio::Value::HIGH}
			},
		}
	);
}

void StepperMotor::apply_state(const ShouldersState& state, ControlGpios& gpios) {
	std::for_each(
		gpios.begin(),
		gpios.end(),
		[&](auto& gpios_iter) {
			auto state_iter = state.find(gpios_iter.first);
			if (state.end() == state_iter) {
				throw std::invalid_argument("invalid state received");
			}
			gpios_iter.second->write_value((*state_iter).second);
		}
	);
}