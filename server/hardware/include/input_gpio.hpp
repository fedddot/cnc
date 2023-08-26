#ifndef	__INPUT_GPIO_HPP__
#define	__INPUT_GPIO_HPP__

#include <cstddef>
#include "gpio.hpp"

namespace hardware {
	class InputGpio: public Gpio {
	public:
		InputGpio(const std::size_t& pin_number);
		virtual Direction get_direction() const override;
	}; // InputGpio
} // namespace hardware
#endif // __INPUT_GPIO_HPP__