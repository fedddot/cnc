#include "raw_data_receiver.hpp"
#include "client_raw_data_sender.hpp"
#include "raw_data_communication_manager.hpp"

using namespace communication;

RawDataCommunicationManager::RawDataCommunicationManager(const std::vector<char>& header, const std::size_t& length_field_size, const std::size_t& max_data_size): 
	CommunicationManager(
		std::shared_ptr<IReceiver<char, const std::vector<char>&>>(new RawDataReceiver(header, length_field_size, max_data_size)), 
		std::shared_ptr<ISender<const std::vector<char>&>>(new ClientRawDataSender(header, length_field_size))
	)
{
}

void RawDataCommunicationManager::start() {

}
void RawDataCommunicationManager::stop() {

} 
bool RawDataCommunicationManager::isRunning() const {
	return false;
}