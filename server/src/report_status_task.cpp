// #include <iostream>
#include "report_status_task.hpp"

using namespace onboard;

ReportStatusTask::ReportStatusTask(const std::string& status) : m_status(status) {

}

ReportStatusTask::McuTaskType ReportStatusTask::getType() const {
	return McuTaskType::REPORT_STATUS;
}

void ReportStatusTask::execute() {
	// std::cout << m_status << std::endl;
}