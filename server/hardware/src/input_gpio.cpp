#include <stdexcept>
#include <cstddef>
#include <algorithm>
#include <stdio.h>

#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "input_gpio.hpp"

using namespace hardware;

InputGpio::InputGpio(const std::size_t& pin_number): Gpio(pin_number) {
	auto pin_number_uint = static_cast<uint>(get_pin_number());
    gpio_set_dir(pin_number_uint, GPIO_IN);
    gpio_pull_up(pin_number_uint);
}

InputGpio::Direction InputGpio::get_direction() const {
	return Direction::IN;
}