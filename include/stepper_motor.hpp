#ifndef	STEPPER_MOTOR_HPP
#define	STEPPER_MOTOR_HPP

#include <map>
#include <memory>
#include <stdexcept>

#include "array.hpp"
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
		using MotorDataGenerator = TaskDataGenerator<TaskData, Tgpio_id>;
		using MotorTaskExecutor = TaskExecutor<void(const TaskData&)>;

		StepperMotor(
			const MotorShoulders& shoulders,
			const StepperMotorStates& states,
			const MotorDataGenerator& data_generator,
			const MotorTaskExecutor& executor
		);

		StepperMotor(const StepperMotor& other) = delete;
		StepperMotor& operator=(const StepperMotor& other) = delete;
		virtual ~StepperMotor() noexcept = default;

		void steps(const Direction& direction, unsigned int steps_num, unsigned int step_duration_ms) const;
	private:
		MotorShoulders m_shoulders;
		StepperMotorStates m_states;
		std::unique_ptr<MotorDataGenerator> m_data_generator;
		std::unique_ptr<MotorTaskExecutor> m_executor;

		TaskData create_gpios_data() const;
		TaskData delete_gpios_data() const;
		TaskData apply_state_data(const StepperMotorState& state) const;
	};

	template <typename Tgpio_id>
	inline StepperMotor<Tgpio_id>::StepperMotor(
		const MotorShoulders& shoulders,
		const StepperMotorStates& states,
		const MotorDataGenerator& data_generator,
		const MotorTaskExecutor& executor
	): m_shoulders(shoulders), m_states(states), m_data_generator(data_generator.clone()), m_executor(executor.clone()) {
		using GpioState = typename StepperMotorState::GpioState;
		m_executor->execute(create_gpios_data());
		const StepperMotorState shutdown_state(
			{
				{Shoulder::LL, GpioState::LOW},
				{Shoulder::LR, GpioState::LOW},
				{Shoulder::HL, GpioState::LOW},
				{Shoulder::HR, GpioState::LOW},
			}
		);
		m_executor->execute(apply_state_data(shutdown_state));
	}

	template <typename Tgpio_id>
	inline void StepperMotor<Tgpio_id>::steps(const Direction& direction, unsigned int steps_num, unsigned int step_duration_ms) const {
		using namespace mcu_server;
		Array tasks;
		for (auto step_index = 0; steps_num > step_index; ++step_index) {
			tasks.push_back(const Data &data)
		}
		throw std::runtime_error("NOT IMPLEMENTED");
	}
}

#endif // STEPPER_MOTOR_HPP