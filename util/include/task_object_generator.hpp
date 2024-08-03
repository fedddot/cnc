#ifndef	TASK_OBJECT_GENERATOR_HPP
#define	TASK_OBJECT_GENERATOR_HPP

#include <stdexcept>
#include <string>

#include "array.hpp"
#include "gpio.hpp"
#include "integer.hpp"
#include "mcu_factory.hpp"
#include "object.hpp"

#include "task_data_generator.hpp"

namespace cnc_utl {
	class TaskObjectGenerator: public cnc::TaskDataGenerator<mcu_server::Object, int, int> {
	public:
		using TaskObject = mcu_server::Object;
		using GpioId = int;
		using TaskId = int;
		using GpioDirection = typename mcu_platform::Gpio::Direction;
		using GpioState = typename mcu_platform::Gpio::State;
		
		TaskObjectGenerator(
			const std::string& task_type_field = "task_type",
			const std::string& gpio_id_field = "gpio_id",
			const std::string& gpio_dir_field = "gpio_dir",
			const std::string& gpio_state_field = "gpio_state",
			const std::string& delay_field = "delay_ms",
			const std::string& tasks_field = "tasks",
			const std::string& task_data_field = "task_data"
		);
		TaskObjectGenerator(const TaskObjectGenerator& other);
		TaskObjectGenerator& operator=(const TaskObjectGenerator& other) = delete;

		TaskObject generate_create_gpio_data(const GpioId& id, const GpioDirection& dir) const override;
		TaskObject generate_delete_gpio_data(const GpioId& id) const override;
		TaskObject generate_set_gpio_data(const GpioId& id, const GpioState& state) const override;
		TaskObject generate_delay_data(const unsigned int delay_ms) const override;
		TaskObject generate_tasks_data(const mcu_server::Array& tasks) const override;
		TaskObject generate_create_persistent_task_data(const TaskId& id, const mcu_server::Data& task_data) const override;
		cnc::TaskDataGenerator<TaskObject, GpioId, TaskId> *clone() const override;
	private:
		const std::string m_task_type_field;
		const std::string m_gpio_id_field;
		const std::string m_gpio_dir_field;
		const std::string m_gpio_state_field;
		const std::string m_delay_field;
		const std::string m_tasks_field;
		const std::string m_task_data_field;

		using McuTaskType = typename mcu_factory::McuFactory<GpioId, int>::TaskType;
	};

	inline TaskObjectGenerator::TaskObjectGenerator(
		const std::string& task_type_field,
		const std::string& gpio_id_field,
		const std::string& gpio_dir_field,
		const std::string& gpio_state_field,
		const std::string& delay_field,
		const std::string& tasks_field,
		const std::string& task_data_field
	): m_task_type_field(task_type_field), m_gpio_id_field(gpio_id_field), m_gpio_dir_field(gpio_dir_field), m_gpio_state_field(gpio_state_field), m_delay_field(delay_field), m_tasks_field(tasks_field), m_task_data_field(task_data_field) {

	}

	inline TaskObjectGenerator::TaskObjectGenerator(const TaskObjectGenerator& other):
		m_task_type_field(other.m_task_type_field),
		m_gpio_id_field(other.m_gpio_id_field),
		m_gpio_dir_field(other.m_gpio_dir_field),
		m_gpio_state_field(other.m_gpio_state_field),
		m_delay_field(other.m_delay_field),
		m_tasks_field(other.m_tasks_field),
		m_task_data_field(other.m_task_data_field) {

	}

	inline TaskObjectGenerator::TaskObject TaskObjectGenerator::generate_create_gpio_data(const GpioId& id, const GpioDirection& dir) const {
		using namespace mcu_server;
		using namespace mcu_platform;
		Object task_data;
		task_data.add(m_task_type_field, Integer(static_cast<int>(McuTaskType::CREATE_GPIO)));
		task_data.add(m_gpio_id_field, Integer(static_cast<int>(id)));
		task_data.add(m_gpio_dir_field, Integer(static_cast<int>(dir)));
		return task_data;
	}

	inline TaskObjectGenerator::TaskObject TaskObjectGenerator::generate_delete_gpio_data(const GpioId& id) const {
		using namespace mcu_server;
		using namespace mcu_platform;
		Object task_data;
		task_data.add(m_task_type_field, Integer(static_cast<int>(McuTaskType::DELETE_GPIO)));
		task_data.add(m_gpio_id_field, Integer(static_cast<int>(id)));
		return task_data;
	}

	inline TaskObjectGenerator::TaskObject TaskObjectGenerator::generate_set_gpio_data(const GpioId& id, const GpioState& state) const {
		using namespace mcu_server;
		using namespace mcu_platform;
		Object task_data;
		task_data.add(m_task_type_field, Integer(static_cast<int>(McuTaskType::SET_GPIO)));
		task_data.add(m_gpio_id_field, Integer(static_cast<int>(id)));
		task_data.add(m_gpio_state_field, Integer(static_cast<int>(state)));
		return task_data;
	}

	inline TaskObjectGenerator::TaskObject TaskObjectGenerator::generate_delay_data(const unsigned int delay_ms) const {
		using namespace mcu_server;
		using namespace mcu_platform;
		Object task_data;
		task_data.add(m_task_type_field, Integer(static_cast<int>(McuTaskType::DELAY)));
		task_data.add(m_delay_field, Integer(static_cast<int>(delay_ms)));
		return task_data;
	}

	inline TaskObjectGenerator::TaskObject TaskObjectGenerator::generate_tasks_data(const mcu_server::Array& tasks) const {
		using namespace mcu_server;
		using namespace mcu_platform;
		Object task_data;
		task_data.add(m_task_type_field, Integer(static_cast<int>(McuTaskType::SEQUENCE)));
		task_data.add(m_tasks_field, tasks);
		return task_data;
	}

	inline TaskObjectGenerator::TaskObject TaskObjectGenerator::generate_create_persistent_task_data(const TaskId& id, const mcu_server::Data& data) const {
		using namespace mcu_server;
		using namespace mcu_platform;
		Object task_data;
		task_data.add(m_task_type_field, Integer(static_cast<int>(McuTaskType::CREATE_PERSISTENT_TASK)));
		task_data.add(m_task_data_field, data);
		return task_data;
	}

	inline cnc::TaskDataGenerator<TaskObjectGenerator::TaskObject, TaskObjectGenerator::GpioId, TaskObjectGenerator::TaskId> *TaskObjectGenerator::clone() const {
		return new TaskObjectGenerator(*this);
	}
}

#endif // TASK_OBJECT_GENERATOR_HPP