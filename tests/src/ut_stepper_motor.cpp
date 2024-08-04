#include "gtest/gtest.h"
#include <iostream>

#include "custom_task_executor.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "json_data_serializer.hpp"
#include "stepper_motor.hpp"

using namespace cnc;
using namespace mcu_platform;
using namespace mcu_server;

using GpioId = int;
using Shoulder = typename StepperMotor::Shoulder;
using GpioState = typename Gpio::State;

TEST(ut_stepper_motor, ctor_dtor_sanity) {
	// GIVEN
	const StepperMotor::Shoulders shoulders {
		{Shoulder::LL, 4},
		{Shoulder::LR, 5},
		{Shoulder::HL, 6},
		{Shoulder::HR, 7}
	};
	const StepperMotor::MotorStates states(
		{
			StepperMotor::MotorState(
				{
					{Shoulder::LL, GpioState::HIGH},
					{Shoulder::LR, GpioState::LOW},
					{Shoulder::HL, GpioState::LOW},
					{Shoulder::HR, GpioState::LOW}
				}
			),
			StepperMotor::MotorState(
				{
					{Shoulder::LL, GpioState::HIGH},
					{Shoulder::LR, GpioState::LOW},
					{Shoulder::HL, GpioState::HIGH},
					{Shoulder::HR, GpioState::LOW}
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
					[](const Data& data){
						std::cout << "Received task data:" << std::endl;
						std::cout << mcu_server_utl::JsonDataSerializer().serialize(data) << std::endl;
					}
				)
			)
		)
	);
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(delete instance_ptr);

	instance_ptr = nullptr;
}

// TEST(ut_stepper_motor, steps_sanity) {
// 	// GIVEN
// 	const StepperMotor::MotorShoulders shoulders {
// 		{Shoulder::LL, 4},
// 		{Shoulder::LR, 5},
// 		{Shoulder::HL, 6},
// 		{Shoulder::HR, 7}
// 	};
// 	const StepperMotorStates states(
// 		{
// 			StepperMotorState(
// 				{
// 					{Shoulder::LL, GpioState::HIGH},
// 					{Shoulder::LR, GpioState::LOW},
// 					{Shoulder::HL, GpioState::LOW},
// 					{Shoulder::HR, GpioState::LOW}
// 				}
// 			),
// 			StepperMotorState(
// 				{
// 					{Shoulder::LL, GpioState::HIGH},
// 					{Shoulder::LR, GpioState::LOW},
// 					{Shoulder::HL, GpioState::LOW},
// 					{Shoulder::HR, GpioState::LOW}
// 				}
// 			)
// 		}
// 	);

// 	// WHEN
// 	StepperMotor<GpioId> instance(
// 		shoulders,
// 		states,
// 		cnc_utl::TaskObjectGenerator(),
// 		cnc_utl::CustomTaskExecutor<void(const StepperMotor::TaskData&)>(
// 			[](const StepperMotor::TaskData& data){
// 				std::cout << "Received task data:" << std::endl;
// 				std::cout << mcu_server_utl::JsonDataSerializer().serialize(data) << std::endl;
// 			}
// 		),
// 		0
// 	);

// 	// THEN
// 	using Direction = typename StepperMotor<GpioId>::Direction;
// 	ASSERT_NO_THROW(instance.steps(Direction::CCW, 3, 30));
// 	ASSERT_NO_THROW(instance.steps(Direction::CW, 7, 40));
// }