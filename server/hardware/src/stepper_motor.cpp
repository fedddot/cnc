#include <algorithm>
#include <stdexcept>
#include <map>
#include <vector>

#include "gpio.hpp"
#include "output_gpio.hpp"

#include "stepper_motor.hpp"

using namespace hardware;

const StepperMotor::ShouldersStates hardware::StepperMotor::m_states(StepperMotor::init_states());

StepperMotor::StepperMotor(const ControlGpios& control_gpios): m_gpios(init_gpios(control_gpios)), m_current_shoulders_state_iter(m_states.begin()), m_is_enabled(false) {

}

void StepperMotor::step(Direction direction) {
	auto next_state_iter = next_state(m_current_shoulders_state_iter, direction);
	apply_state_smoothly(*next_state_iter, *m_current_shoulders_state_iter, m_gpios);
	m_current_shoulders_state_iter = next_state_iter;
}

void StepperMotor::enable() {
	apply_state(*m_current_shoulders_state_iter, m_gpios);
	m_is_enabled = true;
}

void StepperMotor::disable() {
	apply_state(
		ShouldersState(
			{
				{BridgeShoulder::A_LEFT,	Gpio::Value::LOW},
				{BridgeShoulder::A_RIGHT,	Gpio::Value::LOW},
				{BridgeShoulder::B_LEFT,	Gpio::Value::LOW},
				{BridgeShoulder::B_RIGHT,	Gpio::Value::LOW}
			}
		),
		m_gpios
	);
	m_is_enabled = false;
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

StepperMotor::ControlGpios StepperMotor::init_gpios(const ControlGpios& control_gpios) {
	const std::vector<BridgeShoulder> required_shoulders {
		BridgeShoulder::A_LEFT,	BridgeShoulder::A_RIGHT,
		BridgeShoulder::B_LEFT,	BridgeShoulder::B_RIGHT
	};
	std::for_each(
		required_shoulders.begin(),
		required_shoulders.end(),
		[&](const BridgeShoulder& shoulder) {
			auto iter = control_gpios.find(shoulder);
			if (control_gpios.end() == iter) {
				throw std::invalid_argument("required bridge was not found in received ControlGpios object");
			}

			if (nullptr == iter->second) {
				throw std::invalid_argument("invalid OutputGpio");
			}
		}
	);
	return control_gpios;
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

void StepperMotor::apply_state(const ShouldersState& new_state, ControlGpios& gpios) {
	std::for_each(
		gpios.begin(),
		gpios.end(),
		[&](auto& gpios_iter) {
			auto new_state_iter = new_state.find(gpios_iter.first);
			if (new_state.end() == new_state_iter) {
				throw std::invalid_argument("invalid states received");
			}
			gpios_iter.second->write_value((*new_state_iter).second);
		}
	);
}

void StepperMotor::apply_state_smoothly(const ShouldersState& new_state, const ShouldersState& cur_state, ControlGpios& gpios) {
	std::for_each(
		gpios.begin(),
		gpios.end(),
		[&](auto& gpios_iter) {
			auto new_state_iter = new_state.find(gpios_iter.first);
			auto cur_state_iter = cur_state.find(gpios_iter.first);
			if ((new_state.end() == new_state_iter) || (cur_state.end() == cur_state_iter)) {
				throw std::invalid_argument("invalid states received");
			}
			if ((*new_state_iter).second != (*cur_state_iter).second) {
				gpios_iter.second->write_value((*new_state_iter).second);
			}
		}
	);
}