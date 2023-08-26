#ifndef	__GPIO_HPP__
#define	__GPIO_HPP__

#include <vector>
#include <cstddef>

namespace hardware {
	class Gpio {
	public:
		enum { BAD_PIN_NUMBER = static_cast<std::size_t>(-1) };

		enum class Direction: int {
			IN = 0,
			OUT = 1
		};

		enum class Value: int {
			LOW,
			HIGH
		};

		Gpio(const std::size_t& pin_number);
		virtual ~Gpio() noexcept;
		inline std::size_t get_pin_number() const;
		Value read_value() const;
		
		virtual Direction get_direction() const = 0;
	private:
		std::size_t m_pin_number;
		static std::vector<std::size_t> s_aquired_pins_numbers;
	}; // Gpio

	inline std::size_t Gpio::get_pin_number() const {
		return m_pin_number;
	}
} // namespace hardware
#endif // __GPIO_HPP__