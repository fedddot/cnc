#ifndef	__STEPS_TASK_HPP__
#define	__STEPS_TASK_HPP__

#include <string>

#include "stepper_motor.hpp"
#include "object.hpp"
#include "server_task.hpp"

namespace task {
	class StepsTask: public ServerTask {
	public:
		typedef hardware::StepperMotor::Direction Direction;

		StepsTask(const std::string& id, hardware::StepperMotor& stepper_motor, const Direction& direction, unsigned int number_of_steps, unsigned int step_duration_ms);
		StepsTask(const StepsTask& other) = delete;
		StepsTask& operator=(const StepsTask& other) = delete;
		
		virtual void execute() override;
		virtual data::Object report() const override;
	private:
		hardware::StepperMotor& m_stepper_motor;
		const Direction m_direction;
		const unsigned int m_number_of_steps;
		const unsigned int m_step_duration_ms;
		data::Object m_report;
	}; // StepsTask
} // namespace task

#endif // __STEPS_TASK_HPP__