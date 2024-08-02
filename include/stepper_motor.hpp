#ifndef	STEPPER_MOTOR_HPP
#define	STEPPER_MOTOR_HPP

#include <map>
#include <memory>

#include "object.hpp"
#include "stepper_motor_state.hpp"
#include "stepper_motor_states.hpp"
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

		using Shoulder = typename StepperMotorState<Tgpio_id>::Shoulder;
		using MotorShoulders = std::map<Shoulder, Tgpio_id>;
		using TaskData = mcu_server::Object;
		using TaskDataGenerator = TaskDataGenerator<TaskData, Tgpio_id>;
		using StepperMotorStates = StepperMotorStates<Tgpio_id>;
		using TaskExecutor = TaskExecutor<void(const TaskData&)>;

		StepperMotor(
			const MotorShoulders& shoulders,
			const StepperMotorStates& states,
			const TaskDataGenerator& data_generator,
			const TaskExecutor& executor
		);

		StepperMotor(const StepperMotor& other) = delete;
		StepperMotor& operator=(const StepperMotor& other) = delete;
		virtual ~StepperMotor() noexcept = default;

		void steps(const Direction& direction, unsigned int steps_num, unsigned int step_duration_ms) const;
	private:
		MotorShoulders m_shoulders;
		std::unique_ptr<TaskExecutor> m_executor;
		std::unique_ptr<TaskDataGenerator> m_data_generator;
		StepperMotorStates m_states;
	};

	template <typename Tgpio_id>
	inline StepperMotor<Tgpio_id>::StepperMotor(
		const MotorShoulders& shoulders,
		const StepperMotorStates& states,
		const TaskDataGenerator& data_generator,
		const TaskExecutor& executor
	): {
		
	}
	inline void StepperMotor::steps(const Direction& direction, unsigned int steps_num, unsigned int step_duration_ms) const {

	}
}

#endif // STEPPER_MOTOR_HPP