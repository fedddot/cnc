#ifndef	__REGISTER_STEPPER_MOTOR_TASK_HPP__
#define	__REGISTER_STEPPER_MOTOR_TASK_HPP__

#include <string>
#include <memory>

#include "stepper_motor.hpp"
#include "registry.hpp"
#include "object.hpp"
#include "server_task.hpp"

namespace task {
	class RegisterStepperMotorTask: public ServerTask {
	public:
		typedef common::Registry<std::string, std::shared_ptr<hardware::StepperMotor>> StepperMotorRegistry;

		RegisterStepperMotorTask(const std::string& id, const std::string& motor_id, const hardware::StepperMotor::ControlGpios& control_gpios, StepperMotorRegistry& registry);
		RegisterStepperMotorTask(const RegisterStepperMotorTask& other) = delete;
		RegisterStepperMotorTask& operator=(const RegisterStepperMotorTask& other) = delete;
		
		virtual void execute() override;
		virtual data::Object report() const override;
	private:
		std::string m_motor_id;
		hardware::StepperMotor::ControlGpios m_control_gpios;
		StepperMotorRegistry& m_registry;
		data::Object m_report;
	}; // RegisterStepperMotorTask
} // namespace task

#endif // __REGISTER_STEPPER_MOTOR_TASK_HPP__