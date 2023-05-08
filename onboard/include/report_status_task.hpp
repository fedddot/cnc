#ifndef	__REPORT_STATUS_TASK_HPP__
#define	__REPORT_STATUS_TASK_HPP__

#include <string>
#include "imcu_task.hpp"

namespace onboard {
	class ReportStatusTask: public IMcuTask {
	public:
		ReportStatusTask(const std::string& status);
		virtual McuTaskType getType() const override;
		virtual void execute() override;
	private:
		std::string m_status;
	}; // ReportStatusTask

} // onboard

#endif // __REPORT_STATUS_TASK_HPP__