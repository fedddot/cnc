#ifndef	__CLIENT_SENDER_HPP__
#define	__CLIENT_SENDER_HPP__

#include <vector>
#include <string>
#include <cstddef>

#include "idata.hpp"
#include "isender.hpp"
#include "byte_sender.hpp"

namespace communication {
	class ClientSender: public ByteSender {
	public:
		ClientSender(const std::vector<char>& header, const std::size_t& length_field_size, const std::string& port_path);
		ClientSender(const ClientSender& other) = delete;
		ClientSender& operator=(const ClientSender& other) = delete;

		virtual void send(const data::IData& data) override;
		virtual ~ClientSender() noexcept override;
	private:
		enum { BAD_FD = -1 };
		const std::string m_port_path;
		int m_port_fd;

		static int open_port(const std::string& port_path);
		static void config_port(int port_fd);
		static void close_port(int port_fd);
	}; // ClientSender
} // namespace communication
#endif // __CLIENT_SENDER_HPP__