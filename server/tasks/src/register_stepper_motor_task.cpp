#include <stdexcept>
#include <string>
#include <memory>

#include "stepper_motor.hpp"
#include "registry.hpp"
#include "object.hpp"
#include "string.hpp"
#include "server_task.hpp"

#include "register_stepper_motor_task.hpp"

using namespace task;
using namespace data;
using namespace hardware;

RegisterStepperMotorTask::RegisterStepperMotorTask(const std::string& id, const std::string& motor_id, const hardware::StepperMotor::ControlGpios& control_gpios, StepperMotorRegistry& registry): ServerTask(id), m_motor_id(motor_id), m_control_gpios(control_gpios), m_registry(registry) {

}

void RegisterStepperMotorTask::execute() {
	if (m_registry.is_registered(m_motor_id)) {
		m_report.insert({"result", std::shared_ptr<IData>(new String("FAIL"))});
		m_report.insert({"what", std::shared_ptr<IData>(new String("already registered"))});
		return;
	}
	std::shared_ptr<StepperMotor> motor(new StepperMotor(m_control_gpios));

	m_registry.register_member(m_motor_id, motor);
	m_report.insert({"result", std::shared_ptr<IData>(new String("OK"))});
}

data::Object RegisterStepperMotorTask::report() const {
	return m_report;
}