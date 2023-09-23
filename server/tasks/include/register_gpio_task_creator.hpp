#ifndef	__REGISTER_GPIO_TASK_CREATOR_HPP__
#define	__REGISTER_GPIO_TASK_CREATOR_HPP__

#include <string>
#include <memory>

#include "object.hpp"
#include "server_task.hpp"
#include "register_gpio_task.hpp"
#include "icreator.hpp"

namespace task {
	class RegisterGpioTaskCreator: public common::ICreator<std::shared_ptr<ServerTask>, data::Object> {
	public:
		RegisterGpioTaskCreator(RegisterGpioTask::GpioRegistry& registry, const std::string& id_field = "id", const std::string& pin_number_field = "pin_number", const std::string& direction_field = "direction");
		RegisterGpioTaskCreator(const RegisterGpioTaskCreator& other) = default;
		RegisterGpioTaskCreator& operator=(const RegisterGpioTaskCreator& other) = default;

		virtual std::shared_ptr<ServerTask> create(const data::Object& config_data) override;
	private:
		RegisterGpioTask::GpioRegistry& m_registry;
		std::string m_id_field;
		std::string m_pin_number_field;
		std::string m_direction_field;
	}; // RegisterGpioTaskCreator
} // namespace task
#endif // __REGISTER_GPIO_TASK_CREATOR_HPP__