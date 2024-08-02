#ifndef	STEPPER_MOTOR_HPP
#define	STEPPER_MOTOR_HPP

#include <memory>
#include <string>

#include "array.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "integer.hpp"
#include "mcu_client.hpp"
#include "mcu_factory.hpp"
#include "object.hpp"
#include "serializer.hpp"

namespace cnc {
	class StepperMotor {
	public:
		using GpioId = int;
		using McuData = std::string;
		enum class Direction: int {
			CW,
			CCW
		};

		StepperMotor(
			mcu_client::McuClient<McuData> *client,
			const GpioId& ll_gpio,
			const GpioId& lr_gpio,
			const GpioId& hl_gpio,
			const GpioId& hr_gpio
		);
		StepperMotor(const StepperMotor& other) = delete;
		StepperMotor& operator=(const StepperMotor& other) = delete;
		virtual ~StepperMotor() noexcept = default;

		void steps(const Direction& direction, unsigned int steps_num, unsigned int step_duration_ms) const;
		TaskData generate_delete_gpio_data(const GpioId& id) const;
		TaskData generate_set_gpio_data(const GpioId& id, const GpioState& state) const;
		TaskData generate_delay_data(const unsigned int delay_ms) const;
		TaskData generate_tasks_data(const mcu_server::Array& tasks) const;
	private:
		using TaskData = std::string;
		using GpioDirection = typename mcu_platform::Gpio::Direction;
		using GpioState = typename mcu_platform::Gpio::State;
		using TaskDataSerializer = mcu_server::Serializer<TaskData(const mcu_server::Data&)>;

		const std::unique_ptr<TaskDataSerializer> m_serializer;
		const std::string m_task_type_field;
		const std::string m_gpio_id_field;
		const std::string m_gpio_dir_field;
		const std::string m_gpio_state_field;
		const std::string m_delay_field;
		const std::string m_tasks_field;

		using McuTaskType = typename mcu_factory::McuFactory<GpioId>::TaskType;
	};

	inline StepperMotor::StepperMotor(
		const TaskDataSerializer& serializer,
		const std::string& task_type_field,
		const std::string& gpio_id_field,
		const std::string& gpio_dir_field,
		const std::string& gpio_state_field,
		const std::string& delay_field,
		const std::string& tasks_field
	): m_serializer(serializer.clone()), m_task_type_field(task_type_field), m_gpio_id_field(gpio_id_field), m_gpio_dir_field(gpio_dir_field), m_gpio_state_field(gpio_state_field), m_delay_field(delay_field), m_tasks_field(tasks_field) {

	}

	inline StepperMotor::TaskData StepperMotor::generate_create_gpio_data(const GpioId& id, const GpioDirection& dir) const {
		using namespace mcu_server;
		using namespace mcu_platform;
		Object task_data;
		task_data.add(m_task_type_field, Integer(static_cast<int>(McuTaskType::CREATE_GPIO)));
		task_data.add(m_gpio_id_field, Integer(static_cast<int>(id)));
		task_data.add(m_gpio_dir_field, Integer(static_cast<int>(dir)));
		return m_serializer->serialize(task_data);
	}

	inline StepperMotor::TaskData StepperMotor::generate_delete_gpio_data(const GpioId& id) const {
		using namespace mcu_server;
		using namespace mcu_platform;
		Object task_data;
		task_data.add(m_task_type_field, Integer(static_cast<int>(McuTaskType::DELETE_GPIO)));
		task_data.add(m_gpio_id_field, Integer(static_cast<int>(id)));
		return m_serializer->serialize(task_data);
	}

	inline StepperMotor::TaskData StepperMotor::generate_set_gpio_data(const GpioId& id, const GpioState& state) const {
		using namespace mcu_server;
		using namespace mcu_platform;
		Object task_data;
		task_data.add(m_task_type_field, Integer(static_cast<int>(McuTaskType::SET_GPIO)));
		task_data.add(m_gpio_id_field, Integer(static_cast<int>(id)));
		task_data.add(m_gpio_state_field, Integer(static_cast<int>(state)));
		return m_serializer->serialize(task_data);
	}

	inline StepperMotor::TaskData StepperMotor::generate_delay_data(const unsigned int delay_ms) const {
		using namespace mcu_server;
		using namespace mcu_platform;
		Object task_data;
		task_data.add(m_task_type_field, Integer(static_cast<int>(McuTaskType::DELAY)));
		task_data.add(m_delay_field, Integer(static_cast<int>(delay_ms)));
		return m_serializer->serialize(task_data);
	}

	inline StepperMotor::TaskData StepperMotor::generate_tasks_data(const mcu_server::Array& tasks) const {
		using namespace mcu_server;
		using namespace mcu_platform;
		Object task_data;
		task_data.add(m_task_type_field, Integer(static_cast<int>(McuTaskType::SEQUENCE)));
		task_data.add(m_tasks_field, tasks);
		return m_serializer->serialize(task_data);
	}
}

#endif // STEPPER_MOTOR_HPP