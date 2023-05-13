#ifndef	__TASK_FACTORY_HPP__
#define	__TASK_FACTORY_HPP__

#include "dispatcher.hpp"
#include "ilistener.hpp"
#include "message.hpp"
#include "imcu_task.hpp"

namespace onboard {
	class TaskFactory: public common::IListener<const message::Message&> {
	public:
		TaskFactory() = default;
		virtual void onEvent(const message::Message& event) override;
	}; // TaskFactory

} // onboard

#endif // __TASK_FACTORY_HPP__