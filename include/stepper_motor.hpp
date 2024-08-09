#ifndef	STEPPER_MOTOR_HPP
#define	STEPPER_MOTOR_HPP

#include <cstddef>
#include <vector>

#include "creator.hpp"
#include "data.hpp"
#include "delay_allocator.hpp"
#include "integer.hpp"
#include "mcu_factory.hpp"
#include "object.hpp"
#include "state_executor.hpp"
#include "stepper_motor_types.hpp"
#include "task_executor.hpp"

namespace cnc {

	class StepperMotor {
	public:
		enum class Direction: int {
			CW,
			CCW
		};
		using Executor = TaskExecutor<void(const mcu_server::Data&)>;
		using TaskId = int;
		using TaskIdCreator = mcu_server::Creator<TaskId(void)>;
		
		StepperMotor(
			const Shoulders& shoulders,
			const MotorStates& states,
			const Executor& executor,
			const TaskIdCreator& task_id_ctor
		);

		StepperMotor(const StepperMotor& other) = delete;
		StepperMotor& operator=(const StepperMotor& other) = delete;
		virtual ~StepperMotor() noexcept = default;

		void steps(const Direction& direction, unsigned int steps_num, unsigned int step_duration_ms);
	private:
		MotorStates m_states;
		// StatesExecutor m_executor;

		std::size_t m_current_state;
		const MotorState m_shutdown_state;

		std::size_t next_state(const Direction& direction) const;
	};

	inline StepperMotor::StepperMotor(
		const Shoulders& shoulders,
		const MotorStates& states,
		const Executor& executor,
		const TaskIdCreator& task_id_ctor
	):
		m_states(states),
		// m_executor(shoulders, executor, task_id_ctor),
		m_current_state(0UL),
		m_shutdown_state(
			{
				{Shoulder::IN1, GpioState::LOW},
				{Shoulder::IN2, GpioState::LOW},
				{Shoulder::IN3, GpioState::LOW},
				{Shoulder::IN4, GpioState::LOW}
			}
		) {
		
	}

	inline void StepperMotor::steps(const Direction& direction, unsigned int steps_num, unsigned int step_duration_ms) {
		using namespace mcu_server;
		using TaskType = mcu_factory::McuFactory<int, int>::TaskType;

		Array steps_ids;
		DelayAllocator delay(step_duration_ms, *m_pers_task_id_ctor, *m_executor);
		for (auto step = 0; steps_num > step; ++step) {
			for (auto [shoulder, state]: m_states[m_current_state]) {
				auto step_id = m_gpo_allocators.at(shoulder)->set_task_id(state);
				steps_ids.push_back(Integer(step_id));
			}
			steps_ids.push_back(Integer(delay.delay_id()));

			++m_current_state;
			if (m_states.size() <= m_current_state) {
				m_current_state = 0;
			}
		}
		Object execute_tasks;
		execute_tasks.add("task_type", Integer(static_cast<int>(TaskType::EXECUTE_PERSISTENT_TASKS)));
		execute_tasks.add("tasks", steps_ids);
		m_executor->execute(execute_tasks);
	}

}

#endif // STEPPER_MOTOR_HPP