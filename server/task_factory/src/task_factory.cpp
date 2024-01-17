#include "task_factory.hpp"
#include <cstddef>

using namespace task_factory;

TaskFactory::TaskFactory() {

}

Task *TaskFactory::create(const TaskCfg& cfg) const {
	return nullptr;
}