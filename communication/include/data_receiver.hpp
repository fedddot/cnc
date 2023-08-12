#ifndef	__RAW_DATA_RECEIVER_HPP__
#define	__RAW_DATA_RECEIVER_HPP__

#include <string>
#include <cstddef>
#include <vector>

#include "ilistener.hpp"
#include "ireceiver.hpp"

namespace communication {
	class DataReceiver: public IReceiver<char, const std::vector<char>&> {
	public:
		DataReceiver(const std::vector<char>& header, const std::size_t& length_field_size, const std::size_t& max_data_size);
		DataReceiver(const DataReceiver& other) = delete;
		DataReceiver& operator=(const DataReceiver& other) = delete;	

		virtual void onEvent(char event) override;
		virtual inline void set_data_listener(common::IListener<const std::vector<char>&> *data_listener_ptr) override;
		virtual inline common::IListener<const std::vector<char>&> *get_data_listener() override;

		void reset_receiver();
	private:
		std::vector<char> m_header;
		std::size_t m_length_field_size;
		std::size_t m_max_data_size;

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

		static std::size_t deserialize_data_length(const std::vector<char>& data);
		static std::size_t init_length_field_size(const std::size_t& length_field_size);
		
	}; // DataReceiver

	inline void DataReceiver::set_data_listener(common::IListener<const std::vector<char>&> *data_listener_ptr) {
		m_data_listener_ptr = data_listener_ptr;
	}

	inline common::IListener<const std::vector<char>&> *DataReceiver::get_data_listener() {
		return m_data_listener_ptr;
	}
} // namespace data
#endif // __RAW_DATA_RECEIVER_HPP__