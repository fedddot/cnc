#ifndef	__CLIENT_RECEIVER_HPP__
#define	__CLIENT_RECEIVER_HPP__

#include <vector>
#include <thread>
#include <atomic>
#include <string>
#include <cstddef>

#include "byte_receiver.hpp"

namespace communication {
	class ClientReceiver: public ByteReceiver {
	public:
		ClientReceiver(const std::vector<char>& header, const std::size_t& length_field_size, const std::size_t& max_data_size, const std::string& port_path);
		ClientReceiver(const ClientReceiver& other) = delete;
		ClientReceiver& operator=(const ClientReceiver& other) = delete;
		virtual ~ClientReceiver() noexcept override;
	private:
		enum { BAD_FD = -1 };
		std::string m_port_path;
		int m_port_fd;
		int m_polling_timeout;
		
		std::thread m_polling_thread;
		std::atomic_bool m_polling_thread_run_permission;

		static int open_port(const std::string& port_path);
		static void config_port(int port_fd);
		static void close_port(int port_fd);
		
		void poll_fd();
	}; // ClientReceiver
} // namespace communication
#endif // __CLIENT_RECEIVER_HPP__