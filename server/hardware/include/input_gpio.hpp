#ifndef	__INPUT_GPIO_HPP__
#define	__INPUT_GPIO_HPP__

#include <cstddef>
#include "gpio.hpp"

namespace hardware {
	class InputGpio: public Gpio {
	public:
		InputGpio(const std::size_t& pin_number);
		Value read_value() const;
	}; // InputGpio
} // namespace hardware
#endif // __INPUT_GPIO_HPP__