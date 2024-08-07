#ifndef	STEPPER_MOTOR_HPP
#define	STEPPER_MOTOR_HPP

#include <cstddef>
#include <map>
#include <memory>
#include <vector>

#include "creator.hpp"
#include "custom_creator.hpp"
#include "data.hpp"
#include "delay_allocator.hpp"
#include "gpio.hpp"
#include "gpo_allocator.hpp"
#include "integer.hpp"
#include "mcu_factory.hpp"
#include "object.hpp"
#include "task_executor.hpp"

namespace cnc {

	class StepperMotor {
	public:
		enum class Direction: int {
			CW,
			CCW
		};
		enum class Shoulder: int { LL, LR, HL, HR };
		using Shoulders = std::map<Shoulder, int>;
		using GpioState = mcu_platform::Gpio::State;
		using MotorState = std::map<Shoulder, GpioState>;
		using MotorStates = std::vector<MotorState>;
		using Executor = TaskExecutor<void(const mcu_server::Data&)>;
		
		StepperMotor(
			const Shoulders& shoulders,
			const MotorStates& states,
			const Executor& executor
		);

		StepperMotor(const StepperMotor& other) = delete;
		StepperMotor& operator=(const StepperMotor& other) = delete;
		virtual ~StepperMotor() noexcept = default;

		void steps(const Direction& direction, unsigned int steps_num, unsigned int step_duration_ms);
	private:
		std::unique_ptr<Executor> m_executor;
		MotorStates m_states;
		std::size_t m_current_state;
		
		using GpioId = int;
		using TaskId = int;
		using TaskData = mcu_server::Data;

		using GpoAllocators = std::map<Shoulder, std::unique_ptr<GpoAllocator>>;
		GpoAllocators m_gpo_allocators;
		
		using TaskTypes = typename mcu_factory::McuFactory<GpioId, TaskId>::TaskType;
		
		using TaskIdCreator = mcu_server::Creator<TaskId(void)>;
		std::unique_ptr<TaskIdCreator> m_pers_task_id_ctor;
	};

	inline StepperMotor::StepperMotor(
		const Shoulders& shoulders,
		const MotorStates& states,
		const Executor& executor
	): m_states(states), m_executor(executor.clone()), m_current_state(0UL) {

		m_pers_task_id_ctor = std::unique_ptr<TaskIdCreator>(
			new mcu_server_utl::CustomCreator<TaskId(void)>(
				[](void) {
					static TaskId s_id(0);
					return s_id++;
				}
			)
		);
		for (auto [shoulder, gpio_id]: shoulders) {
			m_gpo_allocators.insert(
				{
					shoulder,
					std::make_unique<GpoAllocator>(
						gpio_id, 
						*m_pers_task_id_ctor, 
						*m_executor
					)
				}
			);
		}
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