#include <memory>

#include "gtest/gtest.h"

#include "data.hpp"
#include "state_allocator.hpp"
#include "stepper_motor_types.hpp"

#include "cnc_fixture.hpp"

using namespace cnc;
using namespace cnc_uts;
using namespace mcu_server_utl;
using namespace mcu_server;

TEST_F(CncFixture, sanity) {
	// GIVEN
	const int task_id(12);
	const Shoulders shoulders {
		{Shoulder::IN1, 4},
		{Shoulder::IN2, 5},
		{Shoulder::IN3, 6},
		{Shoulder::IN4, 7}
	};
	const MotorState state {
		{Shoulder::IN1, GpioState::HIGH},
		{Shoulder::IN2, GpioState::LOW},
		{Shoulder::IN3, GpioState::HIGH},
		{Shoulder::IN4, GpioState::LOW}
	};
	StateAllocator allocator(
		task_id,
		shoulders,
		state
	);

	auto create_data = allocator.create_data();
	std::unique_ptr<Data> report(run_data(create_data));

}