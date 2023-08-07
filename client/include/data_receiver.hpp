#ifndef	__DATA_SENDER_HPP__
#define	__DATA_SENDER_HPP__

#include <stdexcept>
#include <string>
#include <cstddef>
#include <vector>
#include "ilistener.hpp"

namespace data {
	class DataReceiver: public common::IListener<char> {
	public:
		class DataReceiverException: public std::exception {
		public:
			DataReceiverException(const std::string& where, const std::string& what, DataReceiver& receiver);
			DataReceiverException(const DataReceiverException& other) = default;
			DataReceiverException& operator=(const DataReceiverException& other) = default;
			virtual const char* what() const noexcept override;
		private:
			std::string m_msg;
		};

		DataReceiver(std::vector<char>& header, const std::size_t& length_field_size, const std::size_t& max_data_size);

		DataReceiver(const DataReceiver& other) = default;
		DataReceiver& operator=(const DataReceiver& other) = default;

		virtual void onEvent(char event) override;
		void set_data_listener(common::IListener<const std::vector<char>&> *data_listener_ptr);
		void reset_receiver();
	private:
		const std::vector<char> m_header;
		const std::size_t m_length_field_size;
		const std::size_t m_max_data_size;

		common::IListener<const std::vector<char>&> *m_data_listener_ptr;

		enum class ReceiverState: int {
			RECEIVING_HEADER,
			RECEIVING_SIZE,
			RECEIVING_DATA
		};

		ReceiverState m_state;
		std::size_t m_data_size;
		std::vector<char> m_data_buff;

		void receive_header(char event);
		void receive_size(char event);
		void receive_data(char event);

		std::size_t deserialize_data_length() const;

		static std::size_t init_length_field_size(const std::size_t& length_field_size);
		
	}; // DataReceiver

	inline const char *DataReceiver::DataReceiverException::what() const noexcept {
		return m_msg.c_str();
	}
} // namespace data
#endif // __DATA_SENDER_HPP__