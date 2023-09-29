#ifndef	__BRIDGE_HPP__
#define	__BRIDGE_HPP__

#include "gpio.hpp"
#include "output_gpio.hpp"

namespace hardware {
	class Bridge {
	public:
		struct State {
		public:
			Gpio::Value left_shoulder_state;
			Gpio::Value right_shoulder_state;
		};
		Bridge(const Gpio::PinNumber& left_shoulder_out, const Gpio::PinNumber& right_shoulder_out, const State& initial_state = {Gpio::Value::LOW, Gpio::Value::LOW});
		Bridge(const Bridge& other) = delete;
		Bridge& operator=(const Bridge& other) = delete;

		~Bridge() noexcept;

		inline State state() const;
		void apply_state(const State& new_state);
	private:
		OutputGpio m_left_shoulder_out;
		OutputGpio m_right_shoulder_out;
		State m_state;
	}; // Bridge

	inline Bridge::State Bridge::state() const {
		return m_state;
	}
} // namespace hardware
#endif // __BRIDGE_HPP__