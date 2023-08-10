#ifndef	__SERVER_DATA_SENDER_HPP__
#define	__SERVER_DATA_SENDER_HPP__

#include <vector>
#include "data_sender.hpp"

namespace data {
	class ServerDataSender: public DataSender {
	public:
		ServerDataSender(const std::vector<char>& header, const std::size_t& length_field_size);

		ServerDataSender(const ServerDataSender& other) = default;
		ServerDataSender& operator=(const ServerDataSender& other) = default;

		virtual void send(const std::vector<char>& data) override;
	}; // ServerDataSender
} // namespace data
#endif // __SERVER_DATA_SENDER_HPP__