#ifndef	__ICNC_TASK_HPP__
#define	__ICNC_TASK_HPP__

#include <memory>

#include "itask.hpp"
#include "idata.hpp"

namespace cnc {
	class ICncTask: public common::ITask {
	public:
		virtual std::shared_ptr<data::IData> getResult() const = 0;
	}; // ICncTask
} // namespace cnc

#endif // __ICNC_TASK_HPP__