#include <iostream>

#include "gtest/gtest.h"

#include "custom_creator.hpp"
#include "custom_task_executor.hpp"
#include "integer.hpp"
#include "json_data_serializer.hpp"
#include "object.hpp"
#include "persistent_task.hpp"
#include "string.hpp"

using namespace mcu_server;
using namespace mcu_server_utl;
using namespace cnc;
using namespace cnc_utl;

TEST(ut_persistent_task, sanity) {
	// GIVEN
	using TaskId = int;
	using TaskData = Object;

	TaskData task_data;
	task_data.add("field1", String("val1"));
	task_data.add("field2", Integer(1));
	const TaskId task_id(12);
	
	CustomCreator<TaskData(const TaskId& id, const TaskData& data)> allocation_ctor(
		[](const TaskId& id, const TaskData& data) {
			TaskData task_data;
			task_data.add("type", String("allocator"));
			task_data.add("task_data", data);
			task_data.add("task_id", Integer(id));
			return task_data;
		}
	);

	CustomCreator<TaskData(const TaskId& id)> deallocation_ctor(
		[](const TaskId& id) {
			TaskData task_data;
			task_data.add("type", String("deallocator"));
			task_data.add("task_id", Integer(id));
			return task_data;
		}
	);

	CustomCreator<TaskId(void)> task_id_ctor(
		[](void) {
			static TaskId s_last_task_id(0);
			return s_last_task_id++;
		}
	);

	cnc_utl::CustomTaskExecutor<void(const TaskData& data)> executor(
		[](const TaskData& data) {
			std::cout << "executing data:" << std::endl;
			std::cout << JsonDataSerializer().serialize(data) << std::endl;
		}
	);
	
	// WHEN
	PersistentTask<TaskId, TaskData> task(
		task_data,
		allocation_ctor,
		deallocation_ctor,
		task_id_ctor,
		executor
	);
	

	// THEN
}