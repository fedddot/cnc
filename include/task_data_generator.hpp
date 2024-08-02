#ifndef	TASK_DATA_GENERATOR_HPP
#define	TASK_DATA_GENERATOR_HPP

#include <chrono>
#include <stdexcept>
#include <thread>
#include <unistd.h>

namespace cnc {
	class TaskDataGenerator {
	public:
		TaskDataGenerator(mcu_ipc::IpcConnection<Tdata> *connection);
		TaskDataGenerator(const TaskDataGenerator& other) = delete;
		TaskDataGenerator& operator=(const TaskDataGenerator& other) = delete;
		virtual ~TaskDataGenerator() noexcept = default;

		Tdata run(const Tdata& data) const;
	private:
		mcu_ipc::IpcConnection<Tdata> *m_connection;
	};

	template <typename Tdata>
	inline TaskDataGenerator<Tdata>::TaskDataGenerator(mcu_ipc::IpcConnection<Tdata> *connection): m_connection(connection) {
		if (!m_connection) {
			throw std::invalid_argument("invalid connection ptr received");
		}
	}

	template <typename Tdata>
	inline Tdata TaskDataGenerator<Tdata>::run(const Tdata& data) const {
		m_connection->send(data);
		while (!m_connection->readable()) {
			std::this_thread::sleep_for(std::chrono::milliseconds(1));
		}
		return m_connection->read();
	}
}

#endif // TASK_DATA_GENERATOR_HPP