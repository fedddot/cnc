#include "gtest/gtest.h"
#include <functional>
#include <map>
#include <memory>

#include "data.hpp"
#include "integer.hpp"
#include "object.hpp"
#include "report.hpp"
#include "task_factory.hpp"

using namespace task_factory;
using namespace cnc_engine;

class TestTask: public TaskFactory::Task {
public:
	TestTask(const TaskFactory::TaskConfigData& cfg): m_cfg(cfg.copy()) {}
	virtual Report execute() override {
		return Report(Report::Result::SUCCESS, *m_cfg);
	}
private:
	std::unique_ptr<TaskFactory::TaskConfigData> m_cfg;
};

class TestTaskCreator: public TaskFactory::TaskCreator {
public:
	virtual TaskFactory::Task *create(const TaskFactory::TaskConfigData& cfg) const {
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
	TaskFactory::Task *task_ptr = nullptr;

	// THEN
	ASSERT_NO_THROW(instance_ptr = new TaskFactory(creators));
	ASSERT_NE(nullptr, instance_ptr);
	ASSERT_NO_THROW(task_ptr = instance_ptr->create(test_cfg));
	ASSERT_NE(nullptr, task_ptr);
	Report report(task_ptr->execute());
}