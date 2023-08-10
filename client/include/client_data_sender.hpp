#ifndef	__CLIENT_DATA_SENDER_HPP__
#define	__CLIENT_DATA_SENDER_HPP__

#include <vector>
#include "data_sender.hpp"
#include "data_receiver.hpp" // delete it

namespace data {
	class ClientDataSender: public DataSender {
	public:
		ClientDataSender(const std::vector<char>& header, const std::size_t& length_field_size);

		ClientDataSender(const ClientDataSender& other) = default;
		ClientDataSender& operator=(const ClientDataSender& other) = default;

		virtual void send(const std::vector<char>& data) override;

		inline void setDataReceiver(DataReceiver *receiver); // delete it
	private:
 		DataReceiver *m_receiver; // delete it
	}; // ClientDataSender

	inline void ClientDataSender::setDataReceiver(DataReceiver *receiver) { // delete it
		m_receiver = receiver;
	}
} // namespace data
#endif // __CLIENT_DATA_SENDER_HPP__