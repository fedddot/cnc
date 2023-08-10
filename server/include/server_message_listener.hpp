#ifndef	__SERVER_MESSAGE_LISTENER_HPP__
#define	__SERVER_MESSAGE_LISTENER_HPP__

#include <vector>

#include "idata_sender.hpp"
#include "ilistener.hpp"

namespace data {
	class ServerMessageListener: public common::IListener<const std::vector<char>&> {
	public:
		ServerMessageListener(data::IDataSender<const std::vector<char>&> *sender);
		virtual void onEvent(const std::vector<char>& event) override;
	private:
		data::IDataSender<const std::vector<char>&> *m_sender;
		static data::IDataSender<const std::vector<char>&> *init_sender(data::IDataSender<const std::vector<char>&> *sender);

		void report_parsing_error(const std::string& what);
	};
} // namespace data
#endif // __SERVER_MESSAGE_LISTENER_HPP__