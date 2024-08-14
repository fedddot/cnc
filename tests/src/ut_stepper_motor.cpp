#include <memory>

#include "gtest/gtest.h"

#include "cnc_fixture.hpp"
#include "custom_creator.hpp"
#include "data.hpp"
#include "stepper_motor.hpp"
#include "stepper_motor_types.hpp"
#include "custom_task_executor.hpp"

using namespace cnc;
using namespace cnc_utl;
using namespace cnc_uts;
using namespace mcu_platform;
using namespace mcu_server;

StepperMotor::MotorStates s_states1 {
	MotorState(
		{
			{Shoulder::IN1, GpioState::HIGH},
			{Shoulder::IN2, GpioState::LOW},
			{Shoulder::IN3, GpioState::HIGH},
			{Shoulder::IN4, GpioState::LOW}
		}
	),
	MotorState(
		{
			{Shoulder::IN1, GpioState::LOW},
			{Shoulder::IN2, GpioState::HIGH},
			{Shoulder::IN3, GpioState::HIGH},
			{Shoulder::IN4, GpioState::LOW}
		}
	),
	MotorState(
		{
			{Shoulder::IN1, GpioState::LOW},
			{Shoulder::IN2, GpioState::HIGH},
			{Shoulder::IN3, GpioState::LOW},
			{Shoulder::IN4, GpioState::HIGH}
		}
	),
	MotorState(
		{
			{Shoulder::IN1, GpioState::HIGH},
			{Shoulder::IN2, GpioState::LOW},
			{Shoulder::IN3, GpioState::LOW},
			{Shoulder::IN4, GpioState::HIGH}
		}
	)
};

StepperMotor::MotorStates s_states2 {
	MotorState(
		{
			{Shoulder::IN1, GpioState::HIGH},
			{Shoulder::IN2, GpioState::LOW},
			{Shoulder::IN3, GpioState::LOW},
			{Shoulder::IN4, GpioState::LOW}
		}
	),
	MotorState(
		{
			{Shoulder::IN1, GpioState::HIGH},
			{Shoulder::IN2, GpioState::LOW},
			{Shoulder::IN3, GpioState::HIGH},
			{Shoulder::IN4, GpioState::LOW}
		}
	),
	MotorState(
		{
			{Shoulder::IN1, GpioState::LOW},
			{Shoulder::IN2, GpioState::LOW},
			{Shoulder::IN3, GpioState::HIGH},
			{Shoulder::IN4, GpioState::LOW}
		}
	),
	MotorState(
		{
			{Shoulder::IN1, GpioState::LOW},
			{Shoulder::IN2, GpioState::HIGH},
			{Shoulder::IN3, GpioState::HIGH},
			{Shoulder::IN4, GpioState::LOW}
		}
	),
	MotorState(
		{
			{Shoulder::IN1, GpioState::LOW},
			{Shoulder::IN2, GpioState::HIGH},
			{Shoulder::IN3, GpioState::LOW},
			{Shoulder::IN4, GpioState::LOW}
		}
	),
	MotorState(
		{
			{Shoulder::IN1, GpioState::LOW},
			{Shoulder::IN2, GpioState::HIGH},
			{Shoulder::IN3, GpioState::LOW},
			{Shoulder::IN4, GpioState::HIGH}
		}
	),
	MotorState(
		{
			{Shoulder::IN1, GpioState::LOW},
			{Shoulder::IN2, GpioState::LOW},
			{Shoulder::IN3, GpioState::LOW},
			{Shoulder::IN4, GpioState::HIGH}
		}
	),
	MotorState(
		{
			{Shoulder::IN1, GpioState::HIGH},
			{Shoulder::IN2, GpioState::LOW},
			{Shoulder::IN3, GpioState::LOW},
			{Shoulder::IN4, GpioState::HIGH}
		}
	)
};

StepperMotor::MotorStates s_states3 {
	MotorState(
		{
			{Shoulder::IN1, GpioState::HIGH},
			{Shoulder::IN2, GpioState::LOW},
			{Shoulder::IN3, GpioState::LOW},
			{Shoulder::IN4, GpioState::LOW}
		}
	),
	MotorState(
		{
			{Shoulder::IN1, GpioState::LOW},
			{Shoulder::IN2, GpioState::LOW},
			{Shoulder::IN3, GpioState::HIGH},
			{Shoulder::IN4, GpioState::LOW}
		}
	),
	MotorState(
		{
			{Shoulder::IN1, GpioState::LOW},
			{Shoulder::IN2, GpioState::HIGH},
			{Shoulder::IN3, GpioState::LOW},
			{Shoulder::IN4, GpioState::LOW}
		}
	),
	MotorState(
		{
			{Shoulder::IN1, GpioState::LOW},
			{Shoulder::IN2, GpioState::LOW},
			{Shoulder::IN3, GpioState::LOW},
			{Shoulder::IN4, GpioState::HIGH}
		}
	)
};

TEST_F(CncFixture, sanity) {
	// GIVEN
	const Shoulders shoulders {
		{Shoulder::IN1, 7},
		{Shoulder::IN2, 6},
		{Shoulder::IN3, 5},
		{Shoulder::IN4, 4}
	};
	
	// WHEN
	StepperMotor *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new StepperMotor(
				shoulders,
				s_states1,
				cnc_utl::CustomTaskExecutor<void(const Data&)>(
					[this](const Data& data){
						std::unique_ptr<Data>(run_data_on_mcu(data));
					}
				),
				mcu_server_utl::CustomCreator<TaskId(void)>(
					[](void) {
						static TaskId counter(0);
						return counter++;
					}
				)
			)
		)
	);

	int iterations_num(5);
	int step_duration_ms(0);
	int steps_number(100);
	while (iterations_num) {
		ASSERT_NO_THROW(instance_ptr->steps(StepperMotor::Direction::CCW, steps_number, step_duration_ms));
		ASSERT_NO_THROW(instance_ptr->steps(StepperMotor::Direction::CW, steps_number, step_duration_ms));
		--iterations_num;
	}

	ASSERT_NO_THROW(delete instance_ptr);

	instance_ptr = nullptr;
}