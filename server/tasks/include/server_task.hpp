#ifndef	__SERVER_TASK_HPP__
#define	__SERVER_TASK_HPP__

#include <string>

#include "object.hpp"
#include "itask.hpp"

namespace task {
	class ServerTask: public ITask {
	public:
		ServerTask(const std::string& id);
		ServerTask(const ServerTask& other) = default;
		ServerTask& operator=(const ServerTask& other) = default;
		~ServerTask() noexcept = default;

		inline std::string id() const;
		virtual data::Object report() const = 0;
	private:
		std::string m_id;
	}; // ServerTask

	inline std::string ServerTask::id() const {
		return m_id;
	}
} // namespace common

#endif // __SERVER_TASK_HPP__