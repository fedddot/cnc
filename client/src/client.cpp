#include <cstddef>
#include <vector>
#include <algorithm>
#include <iostream>

#include "movement_task.hpp"
#include "data_receiver.hpp"
#include "client_data_sender.hpp"
#include "ilistener.hpp"

using namespace task;
using namespace data;
using namespace common;

class DataListener: public IListener<const std::vector<char>&> {
public:
	virtual void onEvent(const std::vector<char>& event) override;
};

void DataListener::onEvent(const std::vector<char>& event) {
	std::cout << "Event received: \"";	
	std::for_each(
		event.begin(),
		event.end(),
		[](const auto& iter) {
			std::cout << iter;
		}
	);
	std::cout << "\"" << std::endl;
}

static inline std::vector<char> strToVector(const std::string& str) {
	std::vector<char> result;
	std::for_each(
		str.begin(),
		str.end(), 
		[&](const auto& iter) { 
			result.push_back(iter); 
		}
	);
	return result;
}

int main(void) {
	const std::vector<char> header(strToVector("msg_header"));
	const std::vector<char> data(strToVector("data"));
	const std::size_t data_length_field_size(2UL);
	const std::size_t max_receiver_capacity(200UL);
	
	DataReceiver receiver(header, 2UL, 200UL);
	DataListener listener;
	receiver.set_data_listener(&listener);

	ClientDataSender sender(header, data_length_field_size);
	sender.setDataReceiver(&receiver);

	sender.send(data);
	sender.send(data);

	
	return 0;
}