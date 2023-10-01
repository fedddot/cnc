#ifndef	__STEPPER_MOTOR_HPP__
#define	__STEPPER_MOTOR_HPP__

#include <map>
#include <vector>

#include "gpio.hpp"
#include "output_gpio.hpp"

namespace hardware {
	class StepperMotor {
	public:
		enum class Direction: int {
			CW = 0,
			CCW = 1
		};
		enum class BridgeShoulder: int {
			A_LEFT,
			A_RIGHT,
			B_LEFT,
			B_RIGHT
		};
		typedef std::map<BridgeShoulder, OutputGpio *> ControlGpios;
		StepperMotor(const ControlGpios& control_gpios);
		~StepperMotor() noexcept = default;
		void step(Direction direction);
		
		void enable();
		void disable();
		inline bool is_enabled() const;
	private:
		typedef std::map<BridgeShoulder, Gpio::Value> ShouldersState;
		typedef std::vector<ShouldersState> ShouldersStates;

		ControlGpios m_gpios;
		ShouldersStates::const_iterator m_current_shoulders_state_iter;
		bool m_is_enabled;

		static const ShouldersStates m_states;

		static ShouldersStates::const_iterator next_state(ShouldersStates::const_iterator from, Direction direction);
		static ControlGpios init_gpios(const ControlGpios& control_gpios);
		static ShouldersStates init_states();
		
		static void apply_state(const ShouldersState& new_state, ControlGpios& gpios);
		static void apply_state_smoothly(const ShouldersState& new_state, const ShouldersState& cur_state, ControlGpios& gpios);
	}; // StepperMotor

	inline bool StepperMotor::is_enabled() const {
		return m_is_enabled;
	}
} // namespace hardware
#endif // __STEPPER_MOTOR_HPP__