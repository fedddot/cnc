#include <stdexcept>
#include <memory>
#include <stdio.h>

#include "pico/stdlib.h"

#include "idata.hpp"
#include "object.hpp"
#include "string.hpp"
#include "movement_task.hpp"
#include "server_movement_task.hpp"

using namespace task;
using namespace data;

ServerMovementTask::ServerMovementTask(const Distance& distance, const Speed& speed, const Axis& axis): MovementTask(distance, speed, axis), m_report(createReport("not executed")) {

}

void ServerMovementTask::execute() {
	m_report = createReport("success");
}

std::shared_ptr<data::IData> ServerMovementTask::createReport(const std::string& result) const {
	Object report;
	report.insert({"result", std::shared_ptr<data::IData>(new String(result))});
	return std::shared_ptr<data::IData>(new Object(report));
}