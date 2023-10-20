#include <iostream>
#include <vector>
#include <mutex>
#include <condition_variable>

#include "ilistener.hpp"

#include "task_result_listener.hpp"

using namespace communication;
using namespace common;

TaskResultListener::TaskResultListener(): m_is_result_data_received(false) {

}

void TaskResultListener::on_event(const std::vector<char>& event) {
	std::unique_lock<std::mutex> lock(m_mux);
	m_result_data = event;
	m_is_result_data_received = true;
	m_cond.notify_all();
}

std::vector<char> TaskResultListener::wait_result() {
	while (true) {
		std::unique_lock<std::mutex> lock(m_mux);
		if (m_is_result_data_received) {
			m_is_result_data_received = false;
			return m_result_data;
		}
		auto wait_result = m_cond.wait_for(lock, std::chrono::seconds(WAITING_TIMEOUT_S));
		if (std::cv_status::timeout == wait_result) {
			throw std::runtime_error("waiting timeout occured!");
		}
	}
}