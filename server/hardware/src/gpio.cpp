#include <stdexcept>
#include <algorithm>
#include <vector>
#include <cstddef>
#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "gpio.hpp"

using namespace hardware;

std::vector<std::size_t> hardware::Gpio::s_aquired_pins_numbers;

Gpio::Gpio(const std::size_t& pin_number): m_pin_number(BAD_PIN_NUMBER) {
	if (BAD_PIN_NUMBER == pin_number) { // add here check if the pin number lays in the range of the platform pin numbers
		throw std::invalid_argument("bad pin number");
	}
	
	auto iter = std::find(s_aquired_pins_numbers.begin(), s_aquired_pins_numbers.end(), pin_number);
	if (s_aquired_pins_numbers.end() != iter) {
		throw std::invalid_argument("gpio with received pin_number = " + std::to_string(pin_number) + " is already aquired");
	}
	m_pin_number = pin_number;
	s_aquired_pins_numbers.push_back(m_pin_number);

	auto pin_number_uint = static_cast<uint>(m_pin_number);
	gpio_init(pin_number_uint);
}

Gpio::~Gpio() noexcept {
	gpio_deinit(static_cast<uint>(m_pin_number));
	auto iter = std::find(s_aquired_pins_numbers.begin(), s_aquired_pins_numbers.end(), m_pin_number);
	if (s_aquired_pins_numbers.end() != iter) {
		s_aquired_pins_numbers.erase(iter);
	}
	m_pin_number = BAD_PIN_NUMBER;
}

Gpio::Value Gpio::read_value() const {
	auto bool_to_gpio_value = [](bool bool_val) { return bool_val ? Value::HIGH : Value::LOW; };
	return bool_to_gpio_value(gpio_get(static_cast<uint>(m_pin_number)));
}