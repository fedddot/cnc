#ifndef	__IMCU_TASK_HPP__
#define	__IMCU_TASK_HPP__

#include "itask.hpp"

namespace onboard {
	class IMcuTask: public common::ITask {
	public:
		enum class McuTaskType : int {
			REPORT_STATUS = 0,
			EMERGENCY_STOP = 1
		};
		virtual McuTaskType getType() const = 0;
	}; // McuResourceManager

} // onboard

#endif // __IMCU_TASK_HPP__