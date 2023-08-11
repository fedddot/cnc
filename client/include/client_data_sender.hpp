#ifndef	__CLIENT_DATA_SENDER_HPP__
#define	__CLIENT_DATA_SENDER_HPP__

#include <vector>
#include <cstddef>

#include "data_sender.hpp"

namespace communication {
	class ClientDataSender: public DataSender {
	public:
		ClientDataSender(const std::vector<char>& header, const std::size_t& length_field_size);
		virtual void send(const std::vector<char>& data) override;
	}; // ClientDataSender
} // namespace communication
#endif // __CLIENT_DATA_SENDER_HPP__