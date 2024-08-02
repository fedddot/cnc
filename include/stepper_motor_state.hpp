#ifndef	STEPPER_MOTOR_STATE_HPP
#define	STEPPER_MOTOR_STATE_HPP

#include <array>
#include <cstddef>
#include <functional>
#include <map>
#include <stdexcept>

#include "gpio.hpp"

namespace cnc {

	template <typename Tgpio_id>
	class StepperMotorState {
	public:
		enum class Shoulder: int { LL, LR, HL, HR };
		using GpioState = typename mcu_platform::Gpio::State;
		using ShouldersState = std::map<Shoulder, GpioState>;
		using ShoulderAction = std::function<void(const Shoulder& shoulder, const Tgpio_id& gpio_id)>;

		StepperMotorState(const ShouldersState& state);
		StepperMotorState(const StepperMotorState& other) = default;
		StepperMotorState& operator=(const StepperMotorState& other) = default;
		virtual ~StepperMotorState() noexcept = default;

		void for_each_shoulder(const ShoulderAction& action) const;
	private:
		ShouldersState m_state;
	};

	template <typename Tgpio_id>
	inline StepperMotorState<Tgpio_id>::StepperMotorState(const ShouldersState& state): m_state(state) {
		enum : std::size_t { SHOULDERS_NUMBER = 4UL };
		static const std::array<Shoulder, SHOULDERS_NUMBER> required_shoulders {Shoulder::LL, Shoulder::LR, Shoulder::HL, Shoulder::HR};
		for (auto shoulder: required_shoulders) {
			auto iter = m_state.find(shoulder);
			if (m_state.end() == iter) {
				throw std::invalid_argument("invalid state received");
			}
		}
	}

	template <typename Tgpio_id>
	inline void StepperMotorState<Tgpio_id>::for_each_shoulder(const ShoulderAction& action) const {
		for (auto iter: m_state) {
			action(iter.first, iter.second);
		}
	}
}

#endif // STEPPER_MOTOR_STATE_HPP