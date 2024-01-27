#include "gtest/gtest.h"
#include <memory>

#include "create_inventory_item_task.hpp"
#include "creator.hpp"
#include "data.hpp"
#include "delete_inventory_item_task.hpp"
#include "integer.hpp"
#include "inventory.hpp"

using namespace basics;
using namespace inventory;
using namespace tasks;
using namespace data;

class TestItemCreator: public basics::Creator<int *, Data> {
public:
	virtual int *create(const Data& cfg) const override {
		return new int(Data::cast<Integer>(cfg).get());
	}
};

TEST(ut_tasks, CreateInventoryItemTask_sanity) {
	// GIVEN
	TestItemCreator creator;
	Inventory<int, int> test_inventory;

	// WHEN
	CreateInventoryItemTask<int, int> instance(
		test_inventory,
		11,
		Integer(2),
		creator
	);
	
	// THEN
	ASSERT_NO_THROW(instance.execute());
	ASSERT_TRUE(test_inventory.contains(11));
	int *item = test_inventory.get(11);
	ASSERT_NE(nullptr, item);
	ASSERT_EQ(2, *item);
}

TEST(ut_tasks, DeleteInventoryItemTask_sanity) {
	// GIVEN
	Inventory<int, int> test_inventory;
	test_inventory.put(0, std::shared_ptr<int>(new int(10)));
	test_inventory.put(1, std::shared_ptr<int>(new int(20)));

	// WHEN
	DeleteInventoryItemTask<int, int> instance(
		test_inventory,
		0
	);
	
	// THEN
	ASSERT_NO_THROW(instance.execute());
	ASSERT_FALSE(test_inventory.contains(0));
}