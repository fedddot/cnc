#ifndef	__RAW_DATA_COMMUNICATION_MANAGER_HPP__
#define	__RAW_DATA_COMMUNICATION_MANAGER_HPP__

#include <string>
#include <vector>
#include <cstddef>
#include <memory>

#include "ilistener.hpp"
#include "ireceiver.hpp"
#include "isender.hpp"
#include "communication_manager.hpp"

namespace communication {
	class RawDataCommunicationManager: public CommunicationManager<char, const std::vector<char>&> {
	public:
		RawDataCommunicationManager(const std::vector<char>& header, const std::size_t& length_field_size, const std::size_t& max_data_size);
		virtual void start() override;
		virtual void stop() override;
		virtual bool isRunning() const override;
	};
} // namespace data
#endif // __RAW_DATA_COMMUNICATION_MANAGER_HPP__