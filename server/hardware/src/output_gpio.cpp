#include "hardware/gpio.h"
#include "gpio.hpp"

#include "output_gpio.hpp"

using namespace hardware;

OutputGpio::OutputGpio(PinNumber pin_number): Gpio(pin_number, Direction::OUT) {
	gpio_set_dir(get_pin_number(), GPIO_OUT);
}

void OutputGpio::write_value(const Value& val) {
	auto value_to_bool = [](const Value& val) { return (Value::HIGH == val) ? true : false; };
	gpio_put(get_pin_number(), value_to_bool(val));
}