#ifndef	__STEPPER_MOTOR_HPP__
#define	__STEPPER_MOTOR_HPP__

#include <map>
#include <memory>
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
		typedef std::map<BridgeShoulder, Gpio::PinNumber> ControlPinLayout;
		StepperMotor(const ControlPinLayout& pin_layout);
		~StepperMotor() noexcept = default;
		void step(Direction direction);
	private:
		typedef std::map<BridgeShoulder, Gpio::Value> ShouldersState;
		typedef std::vector<ShouldersState> ShouldersStates;
		typedef std::map<BridgeShoulder, std::shared_ptr<OutputGpio>> ControlGpios;

		ControlGpios m_gpios;
		ShouldersStates::const_iterator m_current_shoulders_state_iter;


		static const ShouldersStates m_states;

		static ShouldersStates::const_iterator next_state(ShouldersStates::const_iterator from, Direction direction);
		static ControlGpios init_gpios(const ControlPinLayout& pin_layout);
		static ShouldersStates init_states();
		static void apply_state(const ShouldersState& state, ControlGpios& gpios);
	}; // StepperMotor
} // namespace hardware
#endif // __STEPPER_MOTOR_HPP__