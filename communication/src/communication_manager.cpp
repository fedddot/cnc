#include <stdexcept>
#include <vector>
#include <cstddef>
#include <memory>

#include "ilistener.hpp"
#include "ireceiver.hpp"
#include "isender.hpp"
#include "icommunication_manager.hpp"

#include "communication_manager.hpp"

using namespace communication;

CommunicationManager::CommunicationManager(std::shared_ptr<IReceiver<char, const std::vector<char>&>>& receiver, std::shared_ptr<ISender<const std::vector<char>&>>& sender): m_receiver(init_ptr(receiver)), m_sender(init_ptr(sender)) {

}