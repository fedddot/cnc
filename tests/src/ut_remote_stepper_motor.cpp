#include "gtest/gtest.h"

#include "custom_task_executor.hpp"
#include "remote_stepper_motor.hpp"
#include "cnc_fixture.hpp"

using namespace cnc;
using namespace cnc_uts;
using namespace mcu_server;

TEST_F(CncFixture, sanity) {
	// GIVEN
	const Shoulders shoulders {
		{Shoulder::IN0, 7},
		{Shoulder::IN1, 6},
		{Shoulder::IN2, 5},
		{Shoulder::IN3, 4}
	};
	
	const StepperId id(12);
	
	// WHEN
	RemoteStepperMotor *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new RemoteStepperMotor(
				id,
				shoulders,
				states(),
				CustomTaskExecutor<void(const Data&)>(
					[this](const Data& data) {
						run_data_on_mcu(data);
					}
				)
			)
		)
	);

	int iterations_num(5);
	int step_duration_ms(0);
	int steps_number(100);
	while (iterations_num) {
		ASSERT_NO_THROW(instance_ptr->steps(Direction::CCW, steps_number, step_duration_ms));
		ASSERT_NO_THROW(instance_ptr->steps(Direction::CW, steps_number, step_duration_ms));
		--iterations_num;
	}

	ASSERT_NO_THROW(delete instance_ptr);

	instance_ptr = nullptr;
}