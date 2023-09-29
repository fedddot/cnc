#include <stdexcept>

#include "gpio.hpp"
#include "output_gpio.hpp"

#include "bridge.hpp"

using namespace hardware;

Bridge::Bridge(const Gpio::PinNumber& left_shoulder_out, const Gpio::PinNumber& right_shoulder_out, const State& initial_state): m_left_shoulder_out(left_shoulder_out), m_right_shoulder_out(right_shoulder_out), m_state(initial_state) {
	apply_state(m_state);
}

void Bridge::apply_state(const State& new_state) {
	m_left_shoulder_out.write_value(new_state.left_shoulder_state);
	m_right_shoulder_out.write_value(new_state.right_shoulder_state);
	m_state = new_state;
}