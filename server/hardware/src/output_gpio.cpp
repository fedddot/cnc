#include <stdexcept>
#include <cstddef>
#include <algorithm>
#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "output_gpio.hpp"

using namespace hardware;

OutputGpio::OutputGpio(const std::size_t& pin_number): Gpio(pin_number) {
	auto pin_number_uint = static_cast<uint>(get_pin_number());
    gpio_set_dir(pin_number_uint, GPIO_OUT);
}

OutputGpio::Direction OutputGpio::get_direction() const {
	return Direction::OUT;
}

void OutputGpio::write_value(const Value& val) {
	switch (val) {
	case Value::HIGH:
		gpio_put(static_cast<uint>(get_pin_number()), true);
		break;
	
	case Value::LOW:
		gpio_put(static_cast<uint>(get_pin_number()), false);
		break;
	
	default:
		break;
	}
}