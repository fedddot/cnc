#ifndef	STEPPER_MOTOR_HPP
#define	STEPPER_MOTOR_HPP

#include <map>
#include <memory>
#include <stdexcept>

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

		using Shoulder = typename StepperMotorState::Shoulder;
		using MotorShoulders = std::map<Shoulder, Tgpio_id>;
		using TaskData = mcu_server::Object;
		using TaskDataGenerator = TaskDataGenerator<TaskData, Tgpio_id>;
		using StepperMotorStates = StepperMotorStates;
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
		StepperMotorStates m_states;
		std::unique_ptr<TaskDataGenerator> m_data_generator;
		std::unique_ptr<TaskExecutor> m_executor;
	};

	template <typename Tgpio_id>
	inline StepperMotor<Tgpio_id>::StepperMotor(
		const MotorShoulders& shoulders,
		const StepperMotorStates& states,
		const TaskDataGenerator& data_generator,
		const TaskExecutor& executor
	): m_shoulders(shoulders), m_states(states), m_data_generator(data_generator.clone()), m_executor(executor.clone()) {

	}

	template <typename Tgpio_id>
	inline void StepperMotor<Tgpio_id>::steps(const Direction& direction, unsigned int steps_num, unsigned int step_duration_ms) const {
		throw std::runtime_error("NOT IMPLEMENTED");
	}
}

#endif // STEPPER_MOTOR_HPP