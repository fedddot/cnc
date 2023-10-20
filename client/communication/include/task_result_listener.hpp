#ifndef	__TASK_RESULT_LISTENER_HPP__
#define	__TASK_RESULT_LISTENER_HPP__

#include <vector>
#include <mutex>
#include <condition_variable>

#include "ilistener.hpp"

namespace communication {
	class TaskResultListener: public common::IListener<std::vector<char>> {
	public:
		TaskResultListener();
		TaskResultListener(const TaskResultListener& other) = delete;
		TaskResultListener& operator=(const TaskResultListener& other) = delete;
		~TaskResultListener() noexcept = default;

		virtual void on_event(const std::vector<char>& event) override;
		std::vector<char> wait_result();
	private:
		enum {WAITING_TIMEOUT_S = 10};
		std::mutex m_mux;
		std::condition_variable m_cond;
		std::vector<char> m_result_data;
		bool m_is_result_data_received;
	};
} // namespace communication
#endif // __TASK_RESULT_LISTENER_HPP__