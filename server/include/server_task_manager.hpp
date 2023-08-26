#ifndef	__SERVER_TASK_MANAGER_HPP__
#define	__SERVER_TASK_MANAGER_HPP__

#include <string>
#include <list>
#include <memory>

#include "factory.hpp"
#include "isender.hpp"
#include "idata.hpp"
#include "ilistener.hpp"
#include "iserver_task.hpp"

namespace task {
	class ServerTaskManager: public common::IListener<data::IData> {
	public:
		typedef std::shared_ptr<communication::ISender<data::IData>> ServerSenderSmartPtr;
		ServerTaskManager(ServerSenderSmartPtr sender_ptr, const std::string& task_type_key_field = "type");
		virtual void onEvent(const data::IData& event) override;

		std::shared_ptr<IServerTask> dequeue_task();
		bool is_task_pending() const;

		inline communication::ISender<data::IData>& accessSender();
	private:
		typedef common::Factory<std::string, std::shared_ptr<IServerTask>, data::IData> TasksFactory;
		
		typedef std::shared_ptr<common::ICreator<std::shared_ptr<IServerTask>, data::IData>> ServerTaskCreatorSmartPtr;

		ServerSenderSmartPtr m_sender;
		std::string m_task_type_key_field;
		TasksFactory m_task_factory;

		std::list<std::shared_ptr<IServerTask>> m_tasks;

		void init_creators();
		std::string get_task_type(const data::IData& event);

		void report_exception(const std::string& where, const std::string& what);
		void report_task_result(const data::IData& task_result);
	}; // ServerTaskManager

	inline communication::ISender<data::IData>& ServerTaskManager::accessSender() {
		return *m_sender;
	}
} // namespace cnc
#endif // __SERVER_TASK_MANAGER_HPP__