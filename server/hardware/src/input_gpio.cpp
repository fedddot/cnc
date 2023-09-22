#include "hardware/gpio.h"
#include "gpio.hpp"

#include "input_gpio.hpp"

using namespace hardware;

InputGpio::InputGpio(PinNumber pin_number): Gpio(pin_number, Direction::IN) {
    gpio_set_dir(get_pin_number(), GPIO_IN);
    gpio_pull_up(get_pin_number());
}

InputGpio::Value InputGpio::read_value() const {
	auto bool_to_gpio_value = [](bool bool_val) { return bool_val ? Value::HIGH : Value::LOW; };
	return bool_to_gpio_value(gpio_get(get_pin_number()));
}