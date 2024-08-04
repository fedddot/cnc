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
#include "integer.hpp"
#include "object.hpp"
#include "persistent_task.hpp"
#include "stepper_motor_state.hpp"
#include "stepper_motor_states.hpp"
#include "mcu_factory.hpp"
#include "task_executor.hpp"

namespace cnc {

	template <typename Tgpio_id>
	class StepperMotor {
	public:
		enum class Direction: int {
			CW,
			CCW
		};
		enum class Shoulder: int { LL, LR, HL, HR };
		using Shoulders = std::map<Shoulder, Tgpio_id>;
		using GpioState = mcu_platform::Gpio::State;
		using MotorState = std::map<Shoulder, GpioState>;
		using MotorStates = std::vector<MotorState>;
		using Executor = TaskExecutor<void(const mcu_server::Object&)>;
		
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
		
		using TaskId = int;
		using TaskData = mcu_server::Object;

		using MotorStateTask = PersistentTask<TaskId, TaskData>;
		using MotorStateTasksList = std::vector<std::shared_ptr<MotorStateTask>>;
		
		
		// std::vector<std::unique_ptr<MotorPersistentTask>> m_persistent_tasks;

		using TaskTypes = typename mcu_factory::McuFactory<Tgpio_id, TaskId>::TaskType;
		
		using AllocationDataCreator = mcu_server::Creator<TaskData(const TaskId&, const TaskData&)>;
		using DeallocationDataCreator = mcu_server::Creator<TaskData(const TaskId&)>;
		using TaskIdCreator = mcu_server::Creator<TaskId(void)>;
		std::unique_ptr<AllocationDataCreator> m_pers_task_allocator_ctor;
		std::unique_ptr<DeallocationDataCreator> m_pers_task_deallocator_ctor;
		std::unique_ptr<TaskIdCreator> m_pers_task_id_ctor;

		MotorStateTasksList m_state_tasks;
		MotorStateTasksList generate_tasks_list(const MotorStates& states, const Shoulders& shoulders) const;
		// using DelayDataCreator = mcu_server::Creator<TaskData(unsigned int)>;
		// std::unique_ptr<DelayDataCreator> m_delay_data_ctor;

		// void init_persistent_tasks();
		// void uninit_persistent_tasks();
	};

	template <typename Tgpio_id>
	inline StepperMotor<Tgpio_id>::StepperMotor(
		const Shoulders& shoulders,
		const MotorStates& states,
		const Executor& executor
	): m_executor(executor.clone()) {
		m_pers_task_allocator_ctor = std::unique_ptr<AllocationDataCreator>(
			new mcu_server_utl::CustomCreator<TaskData(const TaskId&, const TaskData&)>(
				[](const TaskId& id, const TaskData& task_data) {
					using namespace mcu_server;
					Object task;
					task.add("task_type", Integer(static_cast<int>(TaskTypes::CREATE_PERSISTENT_TASK)));
					task.add("task_id", Integer(static_cast<int>(id)));
					task.add("task_data", task_data);
					return task;
				}
			)
		);
		m_pers_task_deallocator_ctor = std::unique_ptr<DeallocationDataCreator>(
			new mcu_server_utl::CustomCreator<TaskData(const TaskId&)>(
				[](const TaskId& id) {
					using namespace mcu_server;
					Object task;
					task.add("task_type", Integer(static_cast<int>(TaskTypes::DELETE_PERSISTENT_TASK)));
					task.add("task_id", Integer(static_cast<int>(id)));
					return task;
				}
			)
		);
		// m_delay_data_ctor = std::unique_ptr<DelayDataCreator>(
		// 	new mcu_server_utl::CustomCreator<TaskData(unsigned int)>(
		// 		[](unsigned int delay_ms) {
		// 			using namespace mcu_server;
		// 			Object task;
		// 			task.add("task_type", Integer(static_cast<int>(TaskTypes::DELAY)));
		// 			task.add("delay_ms", Integer(static_cast<int>(delay_ms)));
		// 			return task;
		// 		}
		// 	)
		// );
		m_pers_task_id_ctor = std::unique_ptr<TaskIdCreator>(
			new mcu_server_utl::CustomCreator<TaskId(void)>(
				[](void) {
					static TaskId s_id(0);
					return s_id++;
				}
			)
		);

		m_state_tasks = generate_tasks_list(states, shoulders);
		// init_persistent_tasks();
	}

	// template <typename Tgpio_id>
	// inline StepperMotor<Tgpio_id>::~StepperMotor() noexcept {
	// 	void uninit_persistent_tasks();
	// }

	// template <typename Tgpio_id>
	// inline void StepperMotor<Tgpio_id>::steps(const Direction& direction, unsigned int steps_num, unsigned int step_duration_ms) const {
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

	// template <typename Tgpio_id>
	// void StepperMotor<Tgpio_id>::create_persistent_tasks() const {
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

	// template <typename Tgpio_id>
	// void StepperMotor<Tgpio_id>::deallocate_states_appliers() const {
	// 	using namespace mcu_server;
	// 	Array deallocate_tasks;
	// 	m_states.for_each_state(
	// 		[&deallocate_tasks, this](int state_ind, const StepperMotorState& state) {
	// 			deallocate_tasks.push_back(m_data_generator->generate_delete_persistent_task_data(m_task_id_prefix + state_ind));
	// 		}
	// 	);
	// 	m_executor->execute(m_data_generator->generate_tasks_data(deallocate_tasks));
	// }

	// template <typename Tgpio_id>
	// typename StepperMotor<Tgpio_id>::TaskData StepperMotor<Tgpio_id>::create_gpios_data() const {
	// 	using namespace mcu_server;
	// 	using GpioDirection = typename MotorDataGenerator::GpioDirection;
	// 	Array tasks;
	// 	for (auto shoulder: m_shoulders) {
	// 		tasks.push_back(m_data_generator->generate_create_gpio_data(shoulder.second, GpioDirection::OUT));
	// 	}
	// 	return m_data_generator->generate_tasks_data(tasks);
	// }

	// template <typename Tgpio_id>
	// typename StepperMotor<Tgpio_id>::TaskData StepperMotor<Tgpio_id>::delete_gpios_data() const {
	// 	using namespace mcu_server;
	// 	Array tasks;
	// 	for (auto shoulder: m_shoulders) {
	// 		tasks.push_back(m_data_generator->generate_delete_gpio_data(shoulder.second));
	// 	}
	// 	return m_data_generator->generate_tasks_data(tasks);
	// }
	
	// template <typename Tgpio_id>
	// typename StepperMotor<Tgpio_id>::TaskData StepperMotor<Tgpio_id>::apply_state_data(const StepperMotorState& state) const {
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

	// template <typename Tgpio_id>
	// inline StepperMotorState StepperMotor<Tgpio_id>::init_shutdown_state() {
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
	template <typename Tgpio_id>
	typename StepperMotor<Tgpio_id>::MotorStateTasksList StepperMotor<Tgpio_id>::generate_tasks_list(const MotorStates& states, const Shoulders& shoulders) const {
		MotorStateTasksList result;
		for (auto state: states) {
			using namespace mcu_server;
			Array set_gpio_tasks;
			for (auto shoulder_state: state) {
				auto gpio_id = shoulders.at(shoulder_state.first);
				auto gpio_state = shoulder_state.second;
				Object set_gpio_task;
				set_gpio_task.add("task_type", Integer(static_cast<int>(TaskTypes::SET_GPIO)));
				set_gpio_task.add("gpio_id", Integer(static_cast<int>(gpio_id)));
				set_gpio_task.add("gpio_state", Integer(static_cast<int>(gpio_state)));
				set_gpio_tasks.push_back(set_gpio_task);
			}
			Object persistent_task_data;
			persistent_task_data.add("task_type", Integer(static_cast<int>(TaskTypes::SEQUENCE)));
			persistent_task_data.add("tasks", set_gpio_tasks);
			result.push_back(
				std::shared_ptr<MotorStateTask>(
					new MotorStateTask(
						persistent_task_data,
						*m_pers_task_allocator_ctor,
						*m_pers_task_deallocator_ctor,
						*m_pers_task_id_ctor,
						*m_executor
					)
				)
			);
		}
		return result;
	}
}

#endif // STEPPER_MOTOR_HPP