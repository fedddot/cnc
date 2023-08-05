#ifndef	__DATA_SENDER_HPP__
#define	__DATA_SENDER_HPP__

#include <stdexcept>
#include <string>
#include <cstddef>
#include <iostream>
#include "ilistener.hpp"

namespace data {
	class DataReceiver: public common::IListener<char> {
	public:
		class DataReceiverException: public std::exception {
		public:
			DataReceiverException(const std::string& where, const std::string& what);
			DataReceiverException(const DataReceiverException& other) = default;
			DataReceiverException& operator=(const DataReceiverException& other) = default;
			virtual const char* what() const noexcept override;
		private:
			std::string m_msg;
		};

		DataReceiver(const std::string& header, const std::size_t& length_field_size, const std::size_t& max_data_size);

		DataReceiver(const DataReceiver& other) = default;
		DataReceiver& operator=(const DataReceiver& other) = default;

		virtual void onEvent(char event) override;
	private:
		const std::string m_header;
		const std::size_t m_length_field_size;
		const std::size_t m_max_data_size;

		enum ReceiverState {
			RECEIVING_HEADER,
			RECEIVING_SIZE,
			RECEIVING_DATA
		};

		ReceiverState m_state;
		std::iostream m_buffer;

		static std::size_t init_length_field_size(const std::size_t& length_field_size);
		
		static bool capture_header(std::istream& data_stream);

		static std::size_t deserialize_data_length(const std::list<char>& serial_data_length);
	}; // DataReceiver

	inline const char *DataReceiver::DataReceiverException::what() const noexcept {
		return m_msg.c_str();
	}
} // namespace data
#endif // __DATA_SENDER_HPP__