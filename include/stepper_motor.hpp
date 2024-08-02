#ifndef	STEPPER_MOTOR_HPP
#define	STEPPER_MOTOR_HPP

#include <array>
#include <cstddef>
#include <map>

#include "gpio.hpp"
#include "object.hpp"
#include "task_data_generator.hpp"
#include "task_executor.hpp"

namespace cnc {

	template <typename Tgpio_id>
	class StepperMotor {
	public:
		enum class Direction: int {
			CW,
			CCW
		};

		using TaskData = mcu_server::Object;

		StepperMotor(
			const TaskExecutor<void(const TaskData&)>& executor,
			const TaskDataGenerator<TaskData, Tgpio_id> data_generator,
			const Tgpio_id& ll_gpio,
			const Tgpio_id& lr_gpio,
			const Tgpio_id& hl_gpio,
			const Tgpio_id& hr_gpio
		);

		StepperMotor(const StepperMotor& other) = delete;
		StepperMotor& operator=(const StepperMotor& other) = delete;
		virtual ~StepperMotor() noexcept = default;

		void steps(const Direction& direction, unsigned int steps_num, unsigned int step_duration_ms) const;
	private:
		enum class Shoulder: int { LL, LR, HL, HR };
		using GpioState = typename mcu_platform::Gpio::State;
		using MotorShoulders = std::map<Shoulder, Tgpio_id>;
		using MotorState = std::map<Shoulder, GpioState>;
		enum: std::size_t { MOTOR_STATES_NUMBER = 8UL };
		using MotorStates = std::array<const MotorState, MOTOR_STATES_NUMBER>;
		
		std::size_t m_current_state_index;

		static const MotorStates s_states;

		static std::size_t next_state_index(const std::size_t& current_state, const Direction& direction);
	};
}

#endif // STEPPER_MOTOR_HPP