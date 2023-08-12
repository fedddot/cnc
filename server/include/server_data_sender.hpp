#ifndef	__SERVER_DATA_SENDER_HPP__
#define	__SERVER_DATA_SENDER_HPP__

#include <vector>
#include <string>
#include <cstddef>

#include "data_sender.hpp"

namespace communication {
	class ServerDataSender: public DataSender {
	public:
		ServerDataSender(const std::vector<char>& header, const std::size_t& length_field_size, const std::string& port_path);
		ServerDataSender(const ServerDataSender& other) = delete;
		ServerDataSender& operator=(const ServerDataSender& other) = delete;

		virtual void send(const std::vector<char>& data) override;
		virtual ~ServerDataSender() noexcept override;
	private:
		enum { BAD_FD = -1 };
		const std::string m_port_path;
		int m_port_fd;

		static int open_port(const std::string& port_path);
		static void config_port(int port_fd);
		static void close_port(int port_fd);

		std::vector<char> wrap_data(const std::vector<char>& data) const;
	}; // ServerDataSender
} // namespace communication
#endif // __SERVER_DATA_SENDER_HPP__