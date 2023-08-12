#ifndef	__COMMUNICATION_MANAGER_HPP__
#define	__COMMUNICATION_MANAGER_HPP__

#include <vector>

#include "ilistener.hpp"
#include "ireceiver.hpp"
#include "isender.hpp"
#include "icommunication_manager.hpp"

namespace communication {
	class CommunicationManager: public ICommunicationManager<char, const std::vector<char>&> {
	public:
		CommunicationManager(IReceiver<char, const std::vector<char>&>& receiver, ISender<const std::vector<char>&>& sender);

		virtual inline void onEvent(char event) override;
		virtual inline void set_data_listener(common::IListener<const std::vector<char>&> *data_listener_ptr) override;
		virtual inline common::IListener<const std::vector<char>&> *get_data_listener() override;
		virtual inline void send(const std::vector<char>& data) override;
	private:
		IReceiver<char, const std::vector<char>&>& m_receiver;
		ISender<const std::vector<char>&>& m_sender;
	};
 	
	inline void CommunicationManager::onEvent(char event) {
		m_receiver.onEvent(event);
	}

	inline void CommunicationManager::set_data_listener(common::IListener<const std::vector<char>&> *data_listener_ptr) {
		m_receiver.set_data_listener(data_listener_ptr);
	}
	
	inline common::IListener<const std::vector<char>&> *CommunicationManager::get_data_listener() {
		return m_receiver.get_data_listener();
	}
	
	inline void CommunicationManager::send(const std::vector<char>& data) {
		m_sender.send(data);
	}
} // namespace communication
#endif // __COMMUNICATION_MANAGER_HPP__