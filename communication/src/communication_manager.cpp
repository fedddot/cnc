#include <vector>
#include "ireceiver.hpp"
#include "isender.hpp"
#include "communication_manager.hpp"

using namespace communication;

CommunicationManager::CommunicationManager(IReceiver<char, const std::vector<char>&>& receiver, ISender<const std::vector<char>&>& sender): m_receiver(receiver), m_sender(sender) {

}