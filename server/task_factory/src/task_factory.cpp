#include <map>
#include <stdexcept>

#include "integer.hpp"
#include "object.hpp"
#include "creator.hpp"
#include "task_factory.hpp"

using namespace task_factory;
using namespace cnc_engine;

const std::string task_factory::TaskFactory::s_task_type_field_name("task_type");


TaskFactory::TaskFactory(const std::map<TaskType, TaskCreator&>& creators): m_creators(creators) {

}

TaskFactory::TaskType TaskFactory::retrieve_task_type(const TaskConfigData& cfg) {
	const Object& cfg_object = TaskConfigData::cast<Object>(cfg);
	if (!cfg_object.contains(s_task_type_field_name)) {
		throw std::invalid_argument("received cfg object doesn't contain field " + s_task_type_field_name);
	}
	const Integer& int_type = TaskConfigData::cast<Integer>(cfg_object.access(s_task_type_field_name));
	
	switch (int_type.get()) {
	case static_cast<int>(TaskType::CREATE_INVENTORY_ITEM):
		return TaskType::CREATE_INVENTORY_ITEM;
	case static_cast<int>(TaskType::DELETE_INVENTORY_ITEM):
		return TaskType::DELETE_INVENTORY_ITEM;
	}
	throw std::invalid_argument("failed to retrieve task type from the config");
}

TaskFactory::Task *TaskFactory::create(const TaskConfigData& cfg) const {
	auto iter = m_creators.find(retrieve_task_type(cfg));
	if (m_creators.end() == iter) {
		throw std::invalid_argument("creator matching received cfg is not registered");
	}
	return (iter->second).create(cfg);
}