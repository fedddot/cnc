#ifndef	STEPPER_MOTOR_HPP
#define	STEPPER_MOTOR_HPP

#include <cstddef>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

#include "creator.hpp"
#include "custom_creator.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "gpo_allocator.hpp"
#include "mcu_factory.hpp"
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

		void steps(const Direction& direction, unsigned int steps_num, unsigned int step_duration_ms) const;
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
	): m_states(states), m_executor(executor.clone()) {

		m_pers_task_id_ctor = std::unique_ptr<TaskIdCreator>(
			new mcu_server_utl::CustomCreator<TaskId(void)>(
				[](void) {
					static TaskId s_id(0);
					return s_id++;
				}
			)
		);
		for (auto iter: shoulders) {
			m_gpo_allocators.insert(
				{
					iter.first,
					std::make_unique<GpoAllocator>(
						iter.second, 
						*m_pers_task_id_ctor, 
						*m_executor
					)
				}
			);
		}
	}

	inline void StepperMotor::steps(const Direction& direction, unsigned int steps_num, unsigned int step_duration_ms) const {
		throw std::runtime_error("NOT IMPLEMENTED");
	}

}

#endif // STEPPER_MOTOR_HPP