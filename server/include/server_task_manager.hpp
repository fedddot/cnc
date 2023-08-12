#ifndef	__SERVER_TASK_MANAGER_HPP__
#define	__SERVER_TASK_MANAGER_HPP__

#include "ilistener.hpp"
#include "task_factory.hpp"

namespace cnc {
	class ServerTaskManager: public common::IListener<const std::vector<char>&> {
	public:
		typedef TaskFactory::ITaskCreator ITaskCreator;

		ServerTaskManager(TaskFactory& task_factory);
		virtual void onEvent(const std::vector<char>& event) override;
	private:
		TaskFactory& m_task_factory;
	}; // ServerTaskManager
} // namespace cnc
#endif // __SERVER_TASK_MANAGER_HPP__