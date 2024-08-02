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
		virtual ~StepperMotor() noexcept;

		void steps(const Direction& direction, unsigned int steps_num, unsigned int step_duration_ms) const;
	private:
		mutable MotorShoulders m_shoulders;
		mutable StepperMotorStates m_states;
		std::unique_ptr<MotorDataGenerator> m_data_generator;
		std::unique_ptr<MotorTaskExecutor> m_executor;
		const StepperMotorState m_shutdown_state;

		TaskData create_gpios_data() const;
		TaskData delete_gpios_data() const;
		TaskData apply_state_data(const StepperMotorState& state) const;
		static StepperMotorState init_shutdown_state();
	};



	template <typename Tgpio_id>
	inline StepperMotor<Tgpio_id>::StepperMotor(
		const MotorShoulders& shoulders,
		const StepperMotorStates& states,
		const MotorDataGenerator& data_generator,
		const MotorTaskExecutor& executor
	): m_shoulders(shoulders), m_states(states), m_data_generator(data_generator.clone()), m_executor(executor.clone()), m_shutdown_state(init_shutdown_state()) {
		m_executor->execute(create_gpios_data());
		m_executor->execute(apply_state_data(m_shutdown_state));
	}

	template <typename Tgpio_id>
	inline StepperMotor<Tgpio_id>::~StepperMotor() noexcept {
		m_executor->execute(apply_state_data(m_shutdown_state));
		m_executor->execute(delete_gpios_data());
	}

	template <typename Tgpio_id>
	inline void StepperMotor<Tgpio_id>::steps(const Direction& direction, unsigned int steps_num, unsigned int step_duration_ms) const {
		using namespace mcu_server;
		Array tasks;
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
			tasks.push_back(apply_state_data(m_states.current()));
			tasks.push_back(m_data_generator->generate_delay_data(step_duration_ms));
		}
		m_executor->execute(m_data_generator->generate_tasks_data(tasks));
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