#ifndef	__OUTPUT_GPIO_HPP__
#define	__OUTPUT_GPIO_HPP__

#include <cstddef>
#include "gpio.hpp"

namespace hardware {
	class OutputGpio: public Gpio {
	public:
		OutputGpio(const std::size_t& pin_number);
		void write_value(const Value& val);
	}; // OutputGpio
} // namespace hardware
#endif // __OUTPUT_GPIO_HPP__