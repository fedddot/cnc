#include <iostream>

#include "gtest/gtest.h"

#include "custom_creator.hpp"
#include "data.hpp"
#include "gpio.hpp"
#include "gpo_allocator.hpp"
#include "custom_task_executor.hpp"
#include "integer.hpp"
#include "json_data_parser.hpp"
#include "json_data_serializer.hpp"
#include "linux_ipc_connection.hpp"
#include "mcu_client.hpp"
#include "mcu_factory.hpp"
#include "mcu_server.hpp"
#include "object.hpp"

using namespace cnc;
using namespace cnc_utl;
using namespace mcu_server_utl;
using namespace mcu_server;
using namespace mcu_client;
using namespace linux_mcu_ipc;

using TaskType = typename mcu_factory::McuFactory<int, int>::TaskType;

TEST(ut_gpo_allocator, sanity) {
	// GIVEN
	LinuxIpcConnection connection(
		"/dev/ttyACM0",
		LinuxIpcConnection::Baud::BAUD115200,
		"MSG_HEADER",
		"MSG_TAIL",
		1000UL
	);
	McuClient<std::string> client(&connection);
	CustomTaskExecutor<void(const Data&)> executor(
		[&client](const Data& data){
			auto serial_data = mcu_server_utl::JsonDataSerializer().serialize(data);
			std::cout << std::endl << std::endl << "Sending task data:" << std::endl;
			std::cout << serial_data << std::endl;

			auto report = client.run(serial_data);
			std::cout << std::endl << std::endl << "Received report:" << std::endl;
			std::cout << report << std::endl;

			auto parsed_report = std::unique_ptr<Data>(JsonDataParser().parse(report));
			if (0 != Data::cast<Integer>(Data::cast<Object>(*parsed_report).access("result")).get()) {
				throw std::runtime_error("failure report received");
			}
		}
	);
	CustomCreator<int(void)> task_id_ctor(
		[]() {
			static int counter = 0UL;
			return counter++;
		}
	);

	// WHEN
	GpoAllocator allocator(
		25,
		task_id_ctor,
		executor		
	);

	// THEN
	Object execute_high_task;
	execute_high_task.add("task_type", Integer(static_cast<int>(TaskType::EXECUTE_PERSISTENT_TASK)));
	execute_high_task.add("task_id", Integer(allocator.set_task_id(mcu_platform::Gpio::State::HIGH)));
	ASSERT_NO_THROW(executor.execute(execute_high_task));

	Object execute_low_task;
	execute_low_task.add("task_type", Integer(static_cast<int>(TaskType::EXECUTE_PERSISTENT_TASK)));
	execute_low_task.add("task_id", Integer(allocator.set_task_id(mcu_platform::Gpio::State::LOW)));
	ASSERT_NO_THROW(executor.execute(execute_low_task));
}