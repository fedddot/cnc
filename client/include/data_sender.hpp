#ifndef	__DATA_SENDER_HPP__
#define	__DATA_SENDER_HPP__

#include <string>
#include <list>
#include "idata_sender.hpp"

namespace data {
	class DataSender: public IDataSender<std::string> {
	public:
		DataSender(const std::string& header, const std::size_t& length_field_size);

		DataSender(const DataSender& other) = default;
		DataSender& operator=(const DataSender& other) = default;
		~DataSender() noexcept = default;

		void send(const std::string& data);
	private:
		const std::string m_header;
		const std::size_t m_length_field_size;

		static std::size_t init_length_field_size(const std::size_t& length_field_size);

		static std::list<char> serialize_data_length(const std::size_t& data_length, const std::size_t& length_field_size);
	}; // DataSender
} // namespace data
#endif // __DATA_SENDER_HPP__