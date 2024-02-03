#include "gtest/gtest.h"
#include <memory>

#include "data.hpp"
#include "integer.hpp"
#include "inventory.hpp"
#include "inventory_task.hpp"
#include "object.hpp"

using namespace inventory;
using namespace tasks;
using namespace data;

TEST(ut_tasks, InventoryTask_sanity) {
	// GIVEN
	Inventory<int, int> test_inventory;
	test_inventory.put(0, std::shared_ptr<int>(new int(10)));
	test_inventory.put(1, std::shared_ptr<int>(new int(20)));
	Object task_data;
	task_data.add("id", Integer(1));
	task_data.add("val", Integer(30));

	// WHEN
	InventoryTask<int, int> instance(
		&test_inventory,
		[&](Inventory<int, int> *inventory, const Data& data)-> void {
			const Object& data_object = Data::cast<Object>(data);
			auto id = Data::cast<Integer>(data_object.access("id")).get();
			auto val = Data::cast<Integer>(data_object.access("val")).get();
			*(inventory->get(id)) = val;
		},
		task_data
	);
	
	// THEN
	ASSERT_NO_THROW(instance.execute());
	ASSERT_EQ(30, *(test_inventory.get(1)));
}