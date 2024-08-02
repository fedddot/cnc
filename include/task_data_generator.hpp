#ifndef	TASK_DATA_GENERATOR_HPP
#define	TASK_DATA_GENERATOR_HPP

#include <string>

#include "array.hpp"
#include "gpio.hpp"
#include "integer.hpp"
#include "mcu_factory.hpp"
#include "object.hpp"

namespace cnc {
	class TaskDataGenerator {
	public:
		using TaskData = mcu_server::Object;
		using GpioId = int;
		using GpioDirection = typename mcu_platform::Gpio::Direction;
		using GpioState = typename mcu_platform::Gpio::State;
		
		TaskDataGenerator(
			const std::string& task_type_field = "task_type",
			const std::string& gpio_id_field = "gpio_id",
			const std::string& gpio_dir_field = "gpio_dir",
			const std::string& gpio_state_field = "gpio_state",
			const std::string& delay_field = "delay_ms",
			const std::string& tasks_field = "tasks"
		);
		TaskDataGenerator(const TaskDataGenerator& other) = delete;
		TaskDataGenerator& operator=(const TaskDataGenerator& other) = delete;
		virtual ~TaskDataGenerator() noexcept = default;

		TaskData generate_create_gpio_data(const GpioId& id, const GpioDirection& dir) const;
		TaskData generate_delete_gpio_data(const GpioId& id) const;
		TaskData generate_set_gpio_data(const GpioId& id, const GpioState& state) const;
		TaskData generate_delay_data(const unsigned int delay_ms) const;
		TaskData generate_tasks_data(const mcu_server::Array& tasks) const;
	private:
		const std::string m_task_type_field;
		const std::string m_gpio_id_field;
		const std::string m_gpio_dir_field;
		const std::string m_gpio_state_field;
		const std::string m_delay_field;
		const std::string m_tasks_field;

		using McuTaskType = typename mcu_factory::McuFactory<GpioId>::TaskType;
	};

	inline TaskDataGenerator::TaskDataGenerator(
		const std::string& task_type_field,
		const std::string& gpio_id_field,
		const std::string& gpio_dir_field,
		const std::string& gpio_state_field,
		const std::string& delay_field,
		const std::string& tasks_field
	): m_task_type_field(task_type_field), m_gpio_id_field(gpio_id_field), m_gpio_dir_field(gpio_dir_field), m_gpio_state_field(gpio_state_field), m_delay_field(delay_field), m_tasks_field(tasks_field) {

	}

	inline TaskDataGenerator::TaskData TaskDataGenerator::generate_create_gpio_data(const GpioId& id, const GpioDirection& dir) const {
		using namespace mcu_server;
		using namespace mcu_platform;
		Object task_data;
		task_data.add(m_task_type_field, Integer(static_cast<int>(McuTaskType::CREATE_GPIO)));
		task_data.add(m_gpio_id_field, Integer(static_cast<int>(id)));
		task_data.add(m_gpio_dir_field, Integer(static_cast<int>(dir)));
		return task_data;
	}

	inline TaskDataGenerator::TaskData TaskDataGenerator::generate_delete_gpio_data(const GpioId& id) const {
		using namespace mcu_server;
		using namespace mcu_platform;
		Object task_data;
		task_data.add(m_task_type_field, Integer(static_cast<int>(McuTaskType::DELETE_GPIO)));
		task_data.add(m_gpio_id_field, Integer(static_cast<int>(id)));
		return task_data;
	}

	inline TaskDataGenerator::TaskData TaskDataGenerator::generate_set_gpio_data(const GpioId& id, const GpioState& state) const {
		using namespace mcu_server;
		using namespace mcu_platform;
		Object task_data;
		task_data.add(m_task_type_field, Integer(static_cast<int>(McuTaskType::SET_GPIO)));
		task_data.add(m_gpio_id_field, Integer(static_cast<int>(id)));
		task_data.add(m_gpio_state_field, Integer(static_cast<int>(state)));
		return task_data;
	}

	inline TaskDataGenerator::TaskData TaskDataGenerator::generate_delay_data(const unsigned int delay_ms) const {
		using namespace mcu_server;
		using namespace mcu_platform;
		Object task_data;
		task_data.add(m_task_type_field, Integer(static_cast<int>(McuTaskType::DELAY)));
		task_data.add(m_delay_field, Integer(static_cast<int>(delay_ms)));
		return task_data;
	}

	inline TaskDataGenerator::TaskData TaskDataGenerator::generate_tasks_data(const mcu_server::Array& tasks) const {
		using namespace mcu_server;
		using namespace mcu_platform;
		Object task_data;
		task_data.add(m_task_type_field, Integer(static_cast<int>(McuTaskType::SEQUENCE)));
		task_data.add(m_tasks_field, tasks);
		return task_data;
	}
}

#endif // TASK_DATA_GENERATOR_HPP