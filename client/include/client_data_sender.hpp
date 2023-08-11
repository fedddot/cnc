#ifndef	__CLIENT_DATA_SENDER_HPP__
#define	__CLIENT_DATA_SENDER_HPP__

#include <vector>
#include "data_sender.hpp"

namespace data {
	class ClientDataSender: public DataSender {
	public:
		ClientDataSender(const std::vector<char>& header, const std::size_t& length_field_size);

		ClientDataSender(const ClientDataSender& other) = default;
		ClientDataSender& operator=(const ClientDataSender& other) = default;

		virtual void send(const std::vector<char>& data) override;
	private:
	}; // ClientDataSender
} // namespace data
#endif // __CLIENT_DATA_SENDER_HPP__