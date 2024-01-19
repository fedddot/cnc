#include "gtest/gtest.h"

#include "create_inventory_item_task.hpp"
#include "creator.hpp"
#include "data.hpp"
#include "delete_inventory_item_task.hpp"
#include "integer.hpp"
#include "inventory.hpp"
#include "report.hpp"

using namespace basics;
using namespace inventory;
using namespace tasks;
using namespace cnc_engine;

class TestItemCreator: public Creator<int *, Data> {
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
	Report *report = nullptr;
	
	// THEN
	ASSERT_NO_THROW(report = new Report(instance.execute()));
	ASSERT_EQ(Report::Result::SUCCESS, report->result());
	ASSERT_TRUE(test_inventory.contains(11));
	int *item = test_inventory.get(11);
	ASSERT_NE(nullptr, item);
	ASSERT_FALSE(test_inventory.contains(11));
	ASSERT_EQ(2, *item);

	// CLEANUP
	delete item;
	delete report;
}

TEST(ut_tasks, DeleteInventoryItemTask_sanity) {
	// GIVEN
	Inventory<int, int> test_inventory;
	test_inventory.put(0, new int(10));
	test_inventory.put(1, new int(20));

	// WHEN
	DeleteInventoryItemTask<int, int> instance(
		test_inventory,
		0
	);
	Report *report = nullptr;
	
	// THEN
	ASSERT_NO_THROW(report = new Report(instance.execute()));
	ASSERT_NE(nullptr, report);
	ASSERT_EQ(Report::Result::SUCCESS, report->result());
	ASSERT_FALSE(test_inventory.contains(0));
	
	// CLEANUP
	delete report;
}