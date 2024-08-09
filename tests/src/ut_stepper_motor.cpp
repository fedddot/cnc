#include "gtest/gtest.h"
#include <cstddef>
#include <memory>

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

TEST_F(CncFixture, sanity) {
	// GIVEN
	const Shoulders shoulders {
		{Shoulder::IN1, 7},
		{Shoulder::IN2, 6},
		{Shoulder::IN3, 5},
		{Shoulder::IN4, 4}
	};
	const StepperMotor::MotorStates states(
		{
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
		}
	);
	// WHEN
	StepperMotor *instance_ptr(nullptr);

	// THEN
	ASSERT_NO_THROW(
		(
			instance_ptr = new StepperMotor(
				shoulders,
				states,
				cnc_utl::CustomTaskExecutor<void(const Data&)>(
					[this](const Data& data){
						std::unique_ptr<Data>(run_data(data));
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
	ASSERT_NO_THROW(instance_ptr->steps(StepperMotor::Direction::CCW, 300, 30));
	ASSERT_NO_THROW(instance_ptr->steps(StepperMotor::Direction::CW, 300, 30));
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);

	instance_ptr = nullptr;
}