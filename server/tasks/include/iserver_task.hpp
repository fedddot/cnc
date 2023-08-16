#ifndef	__ISERVER_TASK_HPP__
#define	__ISERVER_TASK_HPP__

#include <memory>

#include "idata.hpp"
#include "itask.hpp"

namespace task {
	class IServerTask: ITask {
	public:
		virtual std::shared_ptr<data::IData> report() const = 0;
	}; // IServerTask
} // namespace common

#endif // __ISERVER_TASK_HPP__