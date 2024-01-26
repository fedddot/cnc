#include "gtest/gtest.h"
#include <functional>
#include <map>
#include <memory>

#include "data.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "report.hpp"
#include "task.hpp"
#include "task_factory.hpp"

using namespace task_factory;
using namespace cnc_engine;
using namespace data;
using namespace basics;

class TestTask: public Task {
public:
	TestTask(const Data& cfg): m_cfg(cfg.copy()) {}
	virtual void execute() override {
		
	}
private:
	std::unique_ptr<Data> m_cfg;
};

class TestTaskCreator: public TaskFactory::TaskCreator {
public:
	virtual Task *create(const Data& cfg) const {
		return new TestTask(cfg);
	}
};

TEST(ut_engine, sanity) {
	// GIVEN
	TestTaskCreator creator;
	std::map<TaskFactory::TaskType, TaskFactory::TaskCreator&> creators {
		{ TaskFactory::TaskType::CREATE_INVENTORY_ITEM, std::ref(creator) }
	};
	Object test_cfg;
	test_cfg.add("task_type", Integer(static_cast<int>(TaskFactory::TaskType::CREATE_INVENTORY_ITEM)));

	// WHEN
	TaskFactory *instance_ptr = nullptr;
	Task *task_ptr = nullptr;

	// THEN
	ASSERT_NO_THROW(instance_ptr = new TaskFactory(creators));
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(task_ptr = instance_ptr->create(test_cfg));
	ASSERT_NE(nullptr, task_ptr);
	ASSERT_NO_THROW(task_ptr->execute());
}