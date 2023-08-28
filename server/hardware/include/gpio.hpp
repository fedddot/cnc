#ifndef	__GPIO_HPP__
#define	__GPIO_HPP__

#include <vector>

namespace hardware {
	class Gpio {
	public:
		enum class Direction: int {
			IN = 0,
			OUT = 1
		};
		enum class Value: int {
			LOW,
			HIGH
		};
		typedef unsigned int PinNumber;

		Gpio(PinNumber pin_number, Direction direction);
		virtual ~Gpio() noexcept;
		inline PinNumber get_pin_number() const;
		inline Direction get_direction() const;
	private:
		enum : unsigned int { MIN_PIN_NUMBER = 0, MAX_PIN_NUMBER = 26 };
		PinNumber m_pin_number;
		Direction m_direction;
		static std::vector<PinNumber> s_aquired_pin_numbers;
		static PinNumber aquirePinNumber(PinNumber desired_pin_number);
	}; // Gpio

	inline Gpio::PinNumber Gpio::get_pin_number() const {
		return m_pin_number;
	}

	inline Gpio::Direction Gpio::get_direction() const {
		return m_direction;
	}
} // namespace hardware
#endif // __GPIO_HPP__