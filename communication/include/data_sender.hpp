#ifndef	__RAW_DATA_SENDER_HPP__
#define	__RAW_DATA_SENDER_HPP__

#include <vector>
#include "isender.hpp"

namespace communication {
	class DataSender: public ISender<const std::vector<char>&> {
	public:
		DataSender(const std::vector<char>& header, const std::size_t& length_field_size);
		inline std::vector<char> get_header() const;
		inline std::size_t get_length_field_size() const;
		std::vector<char> get_serial_data_length(const std::size_t& data_length) const;
	private:
		const std::vector<char> m_header;
		const std::size_t m_length_field_size;
		static std::size_t init_length_field_size(const std::size_t& length_field_size);
	}; // DataSender

	inline std::vector<char> DataSender::get_header() const {
		return m_header;
	}

	inline std::size_t DataSender::get_length_field_size() const {
		return m_length_field_size;
	}
} // namespace communication
#endif // __RAW_DATA_SENDER_HPP__