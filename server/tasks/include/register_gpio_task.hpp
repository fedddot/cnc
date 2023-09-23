#ifndef	__REGISTER_GPIO_TASK_HPP__
#define	__REGISTER_GPIO_TASK_HPP__

#include <string>
#include <memory>

#include "gpio.hpp"
#include "registry.hpp"
#include "object.hpp"
#include "server_task.hpp"

namespace task {
	class RegisterGpioTask: public ServerTask {
	public:
		typedef hardware::Gpio::PinNumber PinNumber;
		typedef hardware::Gpio::Direction Direction;
		typedef common::Registry<PinNumber, std::shared_ptr<hardware::Gpio>> GpioRegistry;

		RegisterGpioTask(const std::string& id, const PinNumber& pin_number, const Direction& direction, GpioRegistry& registry);
		RegisterGpioTask(const RegisterGpioTask& other) = delete;
		RegisterGpioTask& operator=(const RegisterGpioTask& other) = delete;
		
		virtual void execute() override;
		virtual data::Object report() const override;
	private:
		data::Object m_report;
		PinNumber m_pin_number;
		Direction m_direction;
		GpioRegistry& m_registry;
	}; // RegisterGpioTask
} // namespace task

#endif // __REGISTER_GPIO_TASK_HPP__