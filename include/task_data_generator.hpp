#ifndef	TASK_DATA_GENERATOR_HPP
#define	TASK_DATA_GENERATOR_HPP

#include <memory>
#include <string>

#include "data.hpp"
#include "gpio.hpp"
#include "serializer.hpp"

namespace cnc {
	class TaskDataGenerator {
	public:
		using TaskData = std::string;
		using GpioId = int;
		using TaskDataSerializer = mcu_server::Serializer<TaskData(const mcu_server::Data&)>;
		
		TaskDataGenerator(
			const TaskDataSerializer& serializer,
			const std::string& task_type_field = "task_type",
			const std::string& gpio_id_field = "gpio_id",
			const std::string& gpio_dir_field = "gpio_dir",
			const std::string& gpio_state_field = "gpio_state",
			const std::string& delay_field = "delay_ms"
		);
		TaskDataGenerator(const TaskDataGenerator& other) = delete;
		TaskDataGenerator& operator=(const TaskDataGenerator& other) = delete;
		virtual ~TaskDataGenerator() noexcept = default;

		TaskData generate_create_gpio_data(int id, const mcu_platform::Gpio::Direction& dir) const;
		TaskData generate_delete_gpio_data(int id) const;
	private:
		std::unique_ptr<TaskDataSerializer> m_serializer;
	};

}

#endif // TASK_DATA_GENERATOR_HPP