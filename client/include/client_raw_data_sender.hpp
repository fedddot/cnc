#ifndef	__CLIENT_RAW_DATA_SENDER_HPP__
#define	__CLIENT_RAW_DATA_SENDER_HPP__

#include <vector>
#include "raw_data_sender.hpp"

namespace communication {
	class ClientRawDataSender: public RawDataSender {
	public:
		ClientRawDataSender(const std::vector<char>& header, const std::size_t& length_field_size);
		virtual void send(const std::vector<char>& data) override;
	}; // ClientRawDataSender
} // namespace communication
#endif // __CLIENT_RAW_DATA_SENDER_HPP__