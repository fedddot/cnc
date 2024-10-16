#include "gtest/gtest.h"
#include <memory>

#include "array.hpp"
#include "custom_task_executor.hpp"
#include "data.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "remote_stepper_motor.hpp"
#include "cnc_fixture.hpp"
#include "stepper_motor_tasks_factory.hpp"

using namespace cnc;
using namespace cnc_uts;
using namespace mcu_server;

TEST_F(CncFixture, sanity) {
	// GIVEN
	const Shoulders shoulders_x {
		{Shoulder::IN0, 22},
		{Shoulder::IN1, 26},
		{Shoulder::IN2, 27},
		{Shoulder::IN3, 28}
	};
	const Shoulders shoulders_y {
		{Shoulder::IN0, 18},
		{Shoulder::IN1, 19},
		{Shoulder::IN2, 20},
		{Shoulder::IN3, 21}
	};
	const Shoulders shoulders_z {
		{Shoulder::IN0, 14},
		{Shoulder::IN1, 15},
		{Shoulder::IN2, 16},
		{Shoulder::IN3, 17}
	};
	
	const StepperId id_x(0);
	const StepperId id_y(1);
	const StepperId id_z(2);
	
	// WHEN
	RemoteStepperMotor motor_x(
		id_x,
		shoulders_x,
		states(),
		CustomTaskExecutor<void(const Data&)>(
			[this](const Data& data) {
				run_data_on_mcu(data);
			}
		)
	);
	RemoteStepperMotor motor_y(
		id_y,
		shoulders_y,
		states(),
		CustomTaskExecutor<void(const Data&)>(
			[this](const Data& data) {
				run_data_on_mcu(data);
			}
		)
	);
	RemoteStepperMotor motor_z(
		id_z,
		shoulders_z,
		states(),
		CustomTaskExecutor<void(const Data&)>(
			[this](const Data& data) {
				run_data_on_mcu(data);
			}
		)
	);

	// THEN
	int step_duration_ms(1);
	int steps_x(500);
	int steps_y(500);
	int steps_z(500);

	Array sequence_data;
	sequence_data.push_back(*std::unique_ptr<Data>(motor_x.generate_steps_data(Direction::CCW, steps_x, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_y.generate_steps_data(Direction::CCW, steps_y, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_z.generate_steps_data(Direction::CCW, steps_z, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_x.generate_steps_data(Direction::CW, steps_x, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_y.generate_steps_data(Direction::CW, steps_y, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_z.generate_steps_data(Direction::CW, steps_z, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_x.generate_steps_data(Direction::CCW, steps_x, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_y.generate_steps_data(Direction::CCW, steps_y, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_z.generate_steps_data(Direction::CCW, steps_z, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_x.generate_steps_data(Direction::CW, steps_x, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_y.generate_steps_data(Direction::CW, steps_y, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_z.generate_steps_data(Direction::CW, steps_z, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_x.generate_steps_data(Direction::CCW, steps_x, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_y.generate_steps_data(Direction::CCW, steps_y, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_z.generate_steps_data(Direction::CCW, steps_z, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_x.generate_steps_data(Direction::CW, steps_x, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_y.generate_steps_data(Direction::CW, steps_y, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_z.generate_steps_data(Direction::CW, steps_z, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_x.generate_steps_data(Direction::CCW, steps_x, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_y.generate_steps_data(Direction::CCW, steps_y, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_z.generate_steps_data(Direction::CCW, steps_z, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_x.generate_steps_data(Direction::CW, steps_x, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_y.generate_steps_data(Direction::CW, steps_y, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_z.generate_steps_data(Direction::CW, steps_z, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_x.generate_steps_data(Direction::CCW, steps_x, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_y.generate_steps_data(Direction::CCW, steps_y, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_z.generate_steps_data(Direction::CCW, steps_z, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_x.generate_steps_data(Direction::CW, steps_x, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_y.generate_steps_data(Direction::CW, steps_y, step_duration_ms)));
	sequence_data.push_back(*std::unique_ptr<Data>(motor_z.generate_steps_data(Direction::CW, steps_z, step_duration_ms)));
	Object task_data;
	task_data.add("task_type", Integer(static_cast<int>(mcu_factory::StepperMotorTasksFactory<int, int>::TaskType::STEPS_SEQUENCE)));
	task_data.add("sequence", sequence_data);
	// ASSERT_NO_THROW(motor_x.steps(Direction::CCW, steps_x, step_duration_ms));
	// ASSERT_NO_THROW(motor_y.steps(Direction::CCW, steps_y, step_duration_ms));
	// ASSERT_NO_THROW(motor_z.steps(Direction::CCW, steps_z, step_duration_ms));
	// ASSERT_NO_THROW(motor_x.steps(Direction::CW, steps_x, step_duration_ms));
	// ASSERT_NO_THROW(motor_y.steps(Direction::CW, steps_y, step_duration_ms));
	// ASSERT_NO_THROW(motor_z.steps(Direction::CW, steps_z, step_duration_ms));

	run_data_on_mcu(task_data);
}