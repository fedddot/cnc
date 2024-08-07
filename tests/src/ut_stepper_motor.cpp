#include "gtest/gtest.h"
#include <iostream>
#include <memory>
#include <stdexcept>

#include "custom_task_executor.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "integer.hpp"
#include "json_data_parser.hpp"
#include "json_data_serializer.hpp"
#include "linux_ipc_connection.hpp"
#include "mcu_client.hpp"
#include "object.hpp"
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
			),
			StepperMotor::MotorState(
				{
					{Shoulder::LL, GpioState::LOW},
					{Shoulder::LR, GpioState::LOW},
					{Shoulder::HL, GpioState::HIGH},
					{Shoulder::HR, GpioState::LOW}
				}
			),
			StepperMotor::MotorState(
				{
					{Shoulder::LL, GpioState::LOW},
					{Shoulder::LR, GpioState::HIGH},
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
						auto serial_data = mcu_server_utl::JsonDataSerializer().serialize(data);
						std::cout << std::endl << std::endl << "Sending task data:" << std::endl;
						std::cout << serial_data << std::endl;

						linux_mcu_ipc::LinuxIpcConnection connection(
							"/dev/ttyACM0",
							linux_mcu_ipc::LinuxIpcConnection::Baud::BAUD115200,
							"MSG_HEADER",
							"MSG_TAIL",
							1000UL
						);
        
						mcu_client::McuClient<std::string> client(&connection);
						auto report = client.run(serial_data);
						std::cout << std::endl << std::endl << "Received report:" << std::endl;
						std::cout << report << std::endl;

						auto parsed_report = std::unique_ptr<Data>(mcu_server_utl::JsonDataParser().parse(report));
						if (0 != Data::cast<Integer>(Data::cast<Object>(*parsed_report).access("result")).get()) {
							throw std::runtime_error("failure report received");
						}
					}
				)
			)
		)
	);
	ASSERT_NO_THROW(instance_ptr->steps(StepperMotor::Direction::CCW, 20, 300));
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