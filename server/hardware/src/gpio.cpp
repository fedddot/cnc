#include <stdexcept>

#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"

#include "gpio.hpp"

using namespace hardware;

Gpio::Gpio(PinNumber pin_number, Direction direction): m_pin_number(init_pin_number(pin_number)), m_direction(direction) {
	gpio_init(static_cast<uint>(m_pin_number));
}

Gpio::~Gpio() noexcept {
	gpio_deinit(static_cast<uint>(m_pin_number));
}

Gpio::PinNumber Gpio::init_pin_number(PinNumber desired_pin_number) {
	if ((MIN_PIN_NUMBER > desired_pin_number) || (MAX_PIN_NUMBER < desired_pin_number)) {
		throw std::invalid_argument("bad pin number");
	}
	return desired_pin_number;
}