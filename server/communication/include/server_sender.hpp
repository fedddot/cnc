#ifndef	__SERVER_SENDER_HPP__
#define	__SERVER_SENDER_HPP__

#include <vector>
#include <cstddef>

#include "idata.hpp"
#include "isender.hpp"
#include "byte_sender.hpp"

namespace communication {
	class ServerDataSender: public ByteSender {
	public:
		ServerDataSender(const std::vector<char>& header, const std::size_t& length_field_size, ISender<std::vector<char>>& sender);
		ServerDataSender(const ServerDataSender& other) = delete;
		ServerDataSender& operator=(const ServerDataSender& other) = delete;

		virtual void send(const data::IData& data) override;
		virtual ~ServerDataSender() noexcept override;
	private:
		ISender<std::vector<char>>& m_sender;
	}; // ServerDataSender
} // namespace communication
#endif // __SERVER_SENDER_HPP__