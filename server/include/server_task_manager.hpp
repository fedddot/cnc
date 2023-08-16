#ifndef	__SERVER_TASK_MANAGER_HPP__
#define	__SERVER_TASK_MANAGER_HPP__

#include <string>
#include <memory>

#include "factory.hpp"
#include "isender.hpp"
#include "idata.hpp"
#include "ilistener.hpp"
#include "icnc_task.hpp"

namespace cnc {
	class ServerTaskManager: public common::IListener<data::IData> {
	public:
		ServerTaskManager(const std::string& task_type_key_field);
		virtual void onEvent(const data::IData& event) override;
	private:
		common::Factory<std::string, std::shared_ptr<ICncTask>, data::IData> m_task_factory;
		std::shared_ptr<communication::ISender<data::IData>> m_sender;
		std::string m_task_type_key_field;

		void init_creators();
		std::string get_task_type(const data::IData& event);

		void report_exception(const std::string& where, const std::string& what);
		void report_task_result(const data::IData& task_result);
	}; // ServerTaskManager
} // namespace cnc
#endif // __SERVER_TASK_MANAGER_HPP__