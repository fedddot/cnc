#ifndef	__REGISTER_STEPPER_MOTOR_TASK_CREATOR_HPP__
#define	__REGISTER_STEPPER_MOTOR_TASK_CREATOR_HPP__

#include <string>
#include <memory>

#include "object.hpp"
#include "server_task.hpp"
#include "registry.hpp"
#include "gpio.hpp"
#include "register_stepper_motor_task.hpp"
#include "icreator.hpp"

namespace task {
	class RegisterStepperMotorTaskCreator: public common::ICreator<std::shared_ptr<ServerTask>, data::Object> {
	public:
		typedef RegisterStepperMotorTask::StepperMotorRegistry StepperMotorRegistry;
		typedef common::Registry<hardware::Gpio::PinNumber, std::shared_ptr<hardware::Gpio>> GpioRegistry;
		
		RegisterStepperMotorTaskCreator(StepperMotorRegistry& stepper_motors_registry, GpioRegistry& gpios_registry, const std::string& id_field = "id", const std::string& motor_id_field = "motor_id", const std::string& control_gpios_field = "control_gpios");
		RegisterStepperMotorTaskCreator(const RegisterStepperMotorTaskCreator& other) = delete;
		RegisterStepperMotorTaskCreator& operator=(const RegisterStepperMotorTaskCreator& other) = delete;

		virtual std::shared_ptr<ServerTask> create(const data::Object& config_data) override;
	private:
		StepperMotorRegistry& m_stepper_motors_registry;
		GpioRegistry& m_gpios_registry;
		std::string m_id_field;
		std::string m_motor_id_field;
		std::string m_control_gpios_field;
	}; // RegisterStepperMotorTaskCreator
} // namespace task
#endif // __REGISTER_STEPPER_MOTOR_TASK_CREATOR_HPP__