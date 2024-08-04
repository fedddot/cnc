#ifndef	STEPPER_MOTOR_HPP
#define	STEPPER_MOTOR_HPP

#include <cstddef>
#include <map>
#include <memory>
#include <stdexcept>
#include <vector>

#include "array.hpp"
#include "creator.hpp"
#include "custom_creator.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "gpo_allocator.hpp"
#include "integer.hpp"
#include "object.hpp"
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
	): m_executor(executor.clone()) {

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

	// template <typename GpioId>
	// inline void StepperMotor<GpioId>::steps(const Direction& direction, unsigned int steps_num, unsigned int step_duration_ms) const {
	// 	using namespace mcu_server;
	// 	MotorPersistentTask delay_task(
	// 		m_delay_data_ctor->create(step_duration_ms),
	// 		*m_pers_task_allocator_ctor,
	// 		*m_pers_task_deallocator_ctor,
	// 		*m_pers_task_id_ctor,
	// 		*m_executor
	// 	);
	// 	Array persistent_tasks_list;
	// 	for (auto step_index = 0; steps_num > step_index; ++step_index) {
	// 		switch (direction) {
	// 		case Direction::CW:
	// 			m_states.move_forward();
	// 			break;
	// 		case Direction::CCW:
	// 			m_states.move_back();
	// 			break;
	// 		default:
	// 			throw std::invalid_argument("unsupported direction");
	// 		}
	// 		persistent_tasks_list.push_back(Integer(m_persistent_tasks[m_states.current()]->id()));
	// 		persistent_tasks_list.push_back(Integer(delay_task.id()));
	// 	}
	// 	Object movement_tasks_data;
	// 	movement_tasks_data.add("task_type", Integer(static_cast<int>(TaskTypes::EXECUTE_PERSISTENT_TASKS)));
	// 	movement_tasks_data.add("tasks", persistent_tasks_list);
	// 	m_executor->execute(movement_tasks_data);
	// }

	// template <typename GpioId>
	// void StepperMotor<GpioId>::create_persistent_tasks() const {
	// 	using namespace mcu_server;
	// 	Array allocate_tasks;
	// 	m_states.for_each_state(
	// 		[&allocate_tasks, this](int state_ind, const StepperMotorState& state) {
	// 			Array set_tasks;
	// 			state.for_each_shoulder(
	// 				[&set_tasks, this](const StepperMotorState::Shoulder& shoulder, const StepperMotorState::GpioState& state) {
	// 					set_tasks.push_back(m_data_generator->generate_set_gpio_data(m_shoulders[shoulder], state));
	// 				}
	// 			);
	// 			allocate_tasks.push_back(
	// 				m_data_generator->generate_create_persistent_task_data(
	// 					m_task_id_prefix + state_ind,
	// 					m_data_generator->generate_tasks_data(set_tasks)
	// 				)
	// 			);
	// 		}
	// 	);
	// 	m_executor->execute(m_data_generator->generate_tasks_data(allocate_tasks));
	// }

	// template <typename GpioId>
	// void StepperMotor<GpioId>::deallocate_states_appliers() const {
	// 	using namespace mcu_server;
	// 	Array deallocate_tasks;
	// 	m_states.for_each_state(
	// 		[&deallocate_tasks, this](int state_ind, const StepperMotorState& state) {
	// 			deallocate_tasks.push_back(m_data_generator->generate_delete_persistent_task_data(m_task_id_prefix + state_ind));
	// 		}
	// 	);
	// 	m_executor->execute(m_data_generator->generate_tasks_data(deallocate_tasks));
	// }

	// template <typename GpioId>
	// typename StepperMotor<GpioId>::TaskData StepperMotor<GpioId>::create_gpios_data() const {
	// 	using namespace mcu_server;
	// 	using GpioDirection = typename MotorDataGenerator::GpioDirection;
	// 	Array tasks;
	// 	for (auto shoulder: m_shoulders) {
	// 		tasks.push_back(m_data_generator->generate_create_gpio_data(shoulder.second, GpioDirection::OUT));
	// 	}
	// 	return m_data_generator->generate_tasks_data(tasks);
	// }

	// template <typename GpioId>
	// typename StepperMotor<GpioId>::TaskData StepperMotor<GpioId>::delete_gpios_data() const {
	// 	using namespace mcu_server;
	// 	Array tasks;
	// 	for (auto shoulder: m_shoulders) {
	// 		tasks.push_back(m_data_generator->generate_delete_gpio_data(shoulder.second));
	// 	}
	// 	return m_data_generator->generate_tasks_data(tasks);
	// }
	
	// template <typename GpioId>
	// typename StepperMotor<GpioId>::TaskData StepperMotor<GpioId>::apply_state_data(const StepperMotorState& state) const {
	// 	using namespace mcu_server;
	// 	using GpioState = typename MotorDataGenerator::GpioState;
	// 	Array tasks;
	// 	state.for_each_shoulder(
	// 		[this, &tasks](const Shoulder& shoulder, const GpioState& state) {
	// 			tasks.push_back(m_data_generator->generate_set_gpio_data(m_shoulders[shoulder], state));
	// 		}
	// 	);
	// 	return m_data_generator->generate_tasks_data(tasks);
	// }

	// template <typename GpioId>
	// inline StepperMotorState StepperMotor<GpioId>::init_shutdown_state() {
	// 	using GpioState = typename StepperMotorState::GpioState;
	// 	return StepperMotorState(
	// 		{
	// 			{Shoulder::LL, GpioState::LOW},
	// 			{Shoulder::LR, GpioState::LOW},
	// 			{Shoulder::HL, GpioState::LOW},
	// 			{Shoulder::HR, GpioState::LOW},
	// 		}
	// 	);
	// }
	// template <typename GpioId>
	// typename StepperMotor<GpioId>::MotorStateTasksList StepperMotor<GpioId>::generate_tasks_list(const MotorStates& states, const Shoulders& shoulders) const {
	// 	MotorStateTasksList result;
	// 	for (auto state: states) {
	// 		using namespace mcu_server;
	// 		Array set_gpio_tasks;
	// 		for (auto shoulder_state: state) {
	// 			auto gpio_id = shoulders.at(shoulder_state.first);
	// 			auto gpio_state = shoulder_state.second;
	// 			Object set_gpio_task;
	// 			set_gpio_task.add("task_type", Integer(static_cast<int>(TaskTypes::SET_GPIO)));
	// 			set_gpio_task.add("gpio_id", Integer(static_cast<int>(gpio_id)));
	// 			set_gpio_task.add("gpio_state", Integer(static_cast<int>(gpio_state)));
	// 			set_gpio_tasks.push_back(set_gpio_task);
	// 		}
	// 		Object persistent_task_data;
	// 		persistent_task_data.add("task_type", Integer(static_cast<int>(TaskTypes::SEQUENCE)));
	// 		persistent_task_data.add("tasks", set_gpio_tasks);
	// 		result.push_back(
	// 			std::shared_ptr<MotorStateTask>(
	// 				new MotorStateTask(
	// 					persistent_task_data,
	// 					*m_pers_task_allocator_ctor,
	// 					*m_pers_task_deallocator_ctor,
	// 					*m_pers_task_id_ctor,
	// 					*m_executor
	// 				)
	// 			)
	// 		);
	// 	}
	// 	return result;
	// }
}

#endif // STEPPER_MOTOR_HPP