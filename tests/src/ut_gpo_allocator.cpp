#include <iostream>

#include "gtest/gtest.h"

#include "custom_creator.hpp"
#include "data.hpp"
#include "gpo_allocator.hpp"
#include "custom_task_executor.hpp"
#include "json_data_serializer.hpp"

using namespace cnc;
using namespace cnc_utl;
using namespace mcu_server_utl;
using namespace mcu_server;

TEST(ut_gpo_allocator, sanity) {


	// WHEN
	GpoAllocator allocator(
		10,
		CustomCreator<int(void)>(
			[]() {
				static int counter = 0UL;
				return counter++;
			}
		),
		CustomTaskExecutor<void(const Data&)>(
			[](const Data& data) {
				std::cout << std::endl << std::endl << JsonDataSerializer().serialize(data) << std::endl << std::endl;
			}
		)
	);
}