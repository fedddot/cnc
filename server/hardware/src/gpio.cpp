#include <stdexcept>
#include <algorithm>
#include <vector>
#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "gpio.hpp"

using namespace hardware;

std::vector<Gpio::PinNumber> hardware::Gpio::s_aquired_pin_numbers;

Gpio::Gpio(PinNumber pin_number, Direction direction): m_pin_number(aquirePinNumber(pin_number)), m_direction(direction) {
	gpio_init(static_cast<uint>(m_pin_number));
}

Gpio::~Gpio() noexcept {
	gpio_deinit(static_cast<uint>(m_pin_number));
	auto iter = std::find(s_aquired_pin_numbers.begin(), s_aquired_pin_numbers.end(), m_pin_number);
	if (s_aquired_pin_numbers.end() != iter) {
		s_aquired_pin_numbers.erase(iter);
	}
	m_pin_number = MAX_PIN_NUMBER + 1;
}

Gpio::PinNumber Gpio::aquirePinNumber(PinNumber desired_pin_number) {

	if ((MIN_PIN_NUMBER > desired_pin_number) || (MAX_PIN_NUMBER < desired_pin_number)) {
		throw std::invalid_argument("bad pin number");
	}
	
	auto iter = std::find(s_aquired_pin_numbers.begin(), s_aquired_pin_numbers.end(), desired_pin_number);
	if (s_aquired_pin_numbers.end() != iter) {
		throw std::invalid_argument("gpio with received pin_number = " + std::to_string(desired_pin_number) + " is already aquired");
	}
	s_aquired_pin_numbers.push_back(desired_pin_number);
	return desired_pin_number;
}