#ifndef	__STEPS_TASK_CREATOR_HPP__
#define	__STEPS_TASK_CREATOR_HPP__

#include <string>
#include <memory>

#include "stepper_motor.hpp"
#include "object.hpp"
#include "server_task.hpp"
#include "registry.hpp"
#include "icreator.hpp"

namespace task {
	class StepsTaskCreator: public common::ICreator<std::shared_ptr<ServerTask>, data::Object> {
	public:
		typedef common::Registry<std::string, std::shared_ptr<hardware::StepperMotor>> StepperMotorRegistry;
		
		StepsTaskCreator(StepperMotorRegistry& stepper_motors_registry, const std::string& id_field = "id", const std::string& motor_id_field = "motor_id", const std::string& direction_field = "direction", const std::string& steps_number_field = "steps_number", const std::string& step_duration_ms_field = "step_duration_ms");
		StepsTaskCreator(const StepsTaskCreator& other) = delete;
		StepsTaskCreator& operator=(const StepsTaskCreator& other) = delete;

		virtual std::shared_ptr<ServerTask> create(const data::Object& config_data) override;
	private:
		StepperMotorRegistry& m_stepper_motors_registry;
		const std::string m_id_field;
		const std::string m_motor_id_field;
		const std::string m_direction_field;
		const std::string m_steps_number_field;
		const std::string m_step_duration_ms_field;
	}; // StepsTaskCreator
} // namespace task
#endif // __STEPS_TASK_CREATOR_HPP__