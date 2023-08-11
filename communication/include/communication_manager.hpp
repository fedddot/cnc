#ifndef	__COMMUNICATION_MANAGER_HPP__
#define	__COMMUNICATION_MANAGER_HPP__

#include <stdexcept>
#include <vector>
#include <cstddef>
#include <memory>

#include "ilistener.hpp"
#include "ireceiver.hpp"
#include "isender.hpp"
#include "icommunication_manager.hpp"

namespace communication {
	class CommunicationManager: public ICommunicationManager<char, const std::vector<char>&> {
	public:
		CommunicationManager(const std::shared_ptr<IReceiver<char, const std::vector<char>&>>& receiver, const std::shared_ptr<ISender<const std::vector<char>&>>& sender);

		virtual inline void onEvent(char event) override;
		virtual inline void set_data_listener(common::IListener<const std::vector<char>&> *data_listener_ptr) override;
		virtual inline common::IListener<const std::vector<char>&> *get_data_listener() override;
		virtual inline void send(const std::vector<char>& data) override;
	private:
		std::shared_ptr<IReceiver<char, const std::vector<char>&>> m_receiver;
		std::shared_ptr<ISender<const std::vector<char>&>> m_sender;
		
		template <class T>
		static inline std::shared_ptr<T> init_ptr(const std::shared_ptr<T>& src);
	};
 	
	inline void CommunicationManager::onEvent(char event) {
		m_receiver->onEvent(event);
	}

	inline void CommunicationManager::set_data_listener(common::IListener<const std::vector<char>&> *data_listener_ptr) {
		m_receiver->set_data_listener(data_listener_ptr);
	}
	
	inline common::IListener<const std::vector<char>&> *CommunicationManager::get_data_listener() {
		return m_receiver->get_data_listener();
	}
	
	inline void CommunicationManager::send(const std::vector<char>& data) {
		m_sender->send(data);
	}

	template <class T>
	inline std::shared_ptr<T> CommunicationManager::init_ptr(const std::shared_ptr<T>& src) {
		if (nullptr == src) {
			throw std::invalid_argument("invalid pointer received");
		}
		return src;
	}
} // namespace data
#endif // __COMMUNICATION_MANAGER_HPP__