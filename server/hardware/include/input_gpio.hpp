#ifndef	__INPUT_GPIO_HPP__
#define	__INPUT_GPIO_HPP__

#include "gpio.hpp"

namespace hardware {
	class InputGpio: public Gpio {
	public:
		InputGpio(PinNumber pin_number);
		Value read_value() const;
	}; // InputGpio
} // namespace hardware
#endif // __INPUT_GPIO_HPP__