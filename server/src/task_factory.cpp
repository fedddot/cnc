#include <stdexcept>
#include "task_factory.hpp"
#include "json_object.hpp"
#include "json_string.hpp"
#include "report_status_task.hpp"

using namespace onboard;
using namespace json;

static IMcuTask::McuTaskType getTaskType(const JsonObject& json_object) {
	auto value = json_object.get("type");
	if (nullptr == value) {
		throw std::invalid_argument("invalid json object received");
	}
	if (IJsonValue::JsonValueType::STRING != value->getType()) {
		throw std::invalid_argument("invalid json object received");
	}
	std::string value_type(((JsonString *)(value.get()))->get());
	if ("REPORT_STATUS" == value_type) {
		return IMcuTask::McuTaskType::REPORT_STATUS;
	}
	if ("EMERGENCY_STOP" == value_type) {
		return IMcuTask::McuTaskType::EMERGENCY_STOP;
	}
	throw std::invalid_argument("received json object defines invalid type");
}

static IMcuTask *generateTask(const JsonObject& json_object) {
	switch (getTaskType(json_object))
	{
	case IMcuTask::McuTaskType::REPORT_STATUS:
		return new ReportStatusTask("OK");
	case IMcuTask::McuTaskType::EMERGENCY_STOP:
		// <<TODO>>
		return nullptr;
	default:
		break;
	}
	return nullptr;
}

void TaskFactory::onEvent(const message::Message& event) {
	JsonObject object;
	object.parse(event.data());
	IMcuTask *task = generateTask(object);
	task->execute();
}
