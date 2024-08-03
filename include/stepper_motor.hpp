#ifndef	STEPPER_MOTOR_HPP
#define	STEPPER_MOTOR_HPP

#include <cstddef>
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
		using TaskId = int;
		using MotorDataGenerator = TaskDataGenerator<TaskData, Tgpio_id, TaskId>;
		using MotorTaskExecutor = TaskExecutor<void(const TaskData&)>;

		StepperMotor(
			const MotorShoulders& shoulders,
			const StepperMotorStates& states,
			const MotorDataGenerator& data_generator,
			const MotorTaskExecutor& executor,
			const TaskId& task_id_prefix
		);

		StepperMotor(const StepperMotor& other) = delete;
		StepperMotor& operator=(const StepperMotor& other) = delete;
		virtual ~StepperMotor() noexcept;

		void steps(const Direction& direction, unsigned int steps_num, unsigned int step_duration_ms) const;
	private:
		mutable MotorShoulders m_shoulders;
		mutable StepperMotorStates m_states;
		std::unique_ptr<MotorDataGenerator> m_data_generator;
		std::unique_ptr<MotorTaskExecutor> m_executor;
		const TaskId m_task_id_prefix;
		const StepperMotorState m_shutdown_state;

		void allocate_states_appliers() const;
		void deallocate_states_appliers() const;
		
		TaskData create_gpios_data() const;
		TaskData delete_gpios_data() const;
		TaskData create_states_appliers_data() const;
		TaskData delete_states_appliers_data() const;
		TaskData apply_state_data(const std::size_t& state_index) const;
		static StepperMotorState init_shutdown_state();
	};



	template <typename Tgpio_id>
	inline StepperMotor<Tgpio_id>::StepperMotor(
		const MotorShoulders& shoulders,
		const StepperMotorStates& states,
		const MotorDataGenerator& data_generator,
		const MotorTaskExecutor& executor,
		const TaskId& task_id_prefix
	): m_shoulders(shoulders), m_states(states), m_data_generator(data_generator.clone()), m_executor(executor.clone()), m_task_id_prefix(task_id_prefix), m_shutdown_state(init_shutdown_state()) {
		m_executor->execute(create_gpios_data());
		m_executor->execute(apply_state_data(m_shutdown_state));
		allocate_states_appliers();
	}

	template <typename Tgpio_id>
	inline StepperMotor<Tgpio_id>::~StepperMotor() noexcept {
		m_executor->execute(apply_state_data(m_shutdown_state));
		m_executor->execute(delete_gpios_data());
	}

	template <typename Tgpio_id>
	inline void StepperMotor<Tgpio_id>::steps(const Direction& direction, unsigned int steps_num, unsigned int step_duration_ms) const {
		using namespace mcu_server;
		const int delay_applier_id = m_task_id_prefix + m_states.size();
		
		Array tasks;
		// add delay persistant task with corresponding 
		tasks.push_back(
			m_data_generator->generate_create_persistent_task_data(step_duration_ms));
		Array persistent_tasks_list;
		for (auto step_index = 0; steps_num > step_index; ++step_index) {
			switch (direction) {
			case Direction::CW:
				m_states.move_forward();
				break;
			case Direction::CCW:
				m_states.move_back();
				break;
			default:
				throw std::invalid_argument("unsupported direction");
			}
			
			const int state_applier_id = m_task_id_prefix + m_states.current();
			
			persistent_tasks_list.push_back(apply_state_data(m_states.current()));
			tasks.push_back(m_data_generator->generate_delay_data(step_duration_ms));
		}
		m_executor->execute(m_data_generator->generate_tasks_data(tasks));
	}

	template <typename Tgpio_id>
	void StepperMotor<Tgpio_id>::allocate_states_appliers() const {
		using namespace mcu_server;
		Array allocate_tasks;
		m_states.for_each_state(
			[&allocate_tasks, this](int state_ind, const StepperMotorState& state) {
				Array set_tasks;
				state.for_each_shoulder(
					[&set_tasks, this](const StepperMotorState::Shoulder& shoulder, const StepperMotorState::GpioState& state) {
						set_tasks.push_back(m_data_generator->generate_set_gpio_data(m_shoulders[shoulder], state));
					}
				);
				allocate_tasks.push_back(
					m_data_generator->generate_create_persistent_task_data(
						m_task_id_prefix + state_ind,
						m_data_generator->generate_tasks_data(set_tasks)
					)
				);
			}
		);
		m_executor->execute(m_data_generator->generate_tasks_data(allocate_tasks));
	}

	template <typename Tgpio_id>
	void StepperMotor<Tgpio_id>::deallocate_states_appliers() const {
		using namespace mcu_server;
		Array deallocate_tasks;
		m_states.for_each_state(
			[&deallocate_tasks, this](int state_ind, const StepperMotorState& state) {
				deallocate_tasks.push_back(m_data_generator->generate_delete_persistent_task_data(m_task_id_prefix + state_ind));
			}
		);
		m_executor->execute(m_data_generator->generate_tasks_data(deallocate_tasks));
	}

	template <typename Tgpio_id>
	typename StepperMotor<Tgpio_id>::TaskData StepperMotor<Tgpio_id>::create_gpios_data() const {
		using namespace mcu_server;
		using GpioDirection = typename MotorDataGenerator::GpioDirection;
		Array tasks;
		for (auto shoulder: m_shoulders) {
			tasks.push_back(m_data_generator->generate_create_gpio_data(shoulder.second, GpioDirection::OUT));
		}
		return m_data_generator->generate_tasks_data(tasks);
	}

	template <typename Tgpio_id>
	typename StepperMotor<Tgpio_id>::TaskData StepperMotor<Tgpio_id>::delete_gpios_data() const {
		using namespace mcu_server;
		Array tasks;
		for (auto shoulder: m_shoulders) {
			tasks.push_back(m_data_generator->generate_delete_gpio_data(shoulder.second));
		}
		return m_data_generator->generate_tasks_data(tasks);
	}
	
	template <typename Tgpio_id>
	typename StepperMotor<Tgpio_id>::TaskData StepperMotor<Tgpio_id>::apply_state_data(const StepperMotorState& state) const {
		using namespace mcu_server;
		using GpioState = typename MotorDataGenerator::GpioState;
		Array tasks;
		state.for_each_shoulder(
			[this, &tasks](const Shoulder& shoulder, const GpioState& state) {
				tasks.push_back(m_data_generator->generate_set_gpio_data(m_shoulders[shoulder], state));
			}
		);
		return m_data_generator->generate_tasks_data(tasks);
	}

	template <typename Tgpio_id>
	inline StepperMotorState StepperMotor<Tgpio_id>::init_shutdown_state() {
		using GpioState = typename StepperMotorState::GpioState;
		return StepperMotorState(
			{
				{Shoulder::LL, GpioState::LOW},
				{Shoulder::LR, GpioState::LOW},
				{Shoulder::HL, GpioState::LOW},
				{Shoulder::HR, GpioState::LOW},
			}
		);
	}
}

#endif // STEPPER_MOTOR_HPP