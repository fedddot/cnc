#include "json_object.hpp"
#include "json_string.hpp"
#include "uart_message_manager.hpp"
#include "list.hpp"
#include "pair.hpp"
#include "shared_ptr.hpp"

using namespace data;
using namespace json;
using namespace cnc_system;
using namespace memory;

#include <stdlib.h>
#include <stddef.h>

void *operator new(size_t size) {
	void *dataPtr = malloc(size);
	if (nullptr == dataPtr) {
		// TODO: report a problem into the log
	}
	return dataPtr;
}

void *operator new[](size_t size) {
	void *dataPtr = malloc(size);
	if (nullptr == dataPtr) {
		// TODO: report a problem into the log
	}
	return dataPtr;
}

void operator delete(void *ptr) noexcept {
	if (nullptr != ptr) {
		free(ptr);
	}
}

void operator delete[](void *ptr) noexcept {
	if (nullptr != ptr) {
		free(ptr);
	}
}

int main(void) {
	JsonObject msg;
	msg.push_back(Pair<String, SharedPtr<IJsonValue>>("msg", SharedPtr<IJsonValue>(new JsonString("Hi from the Controller!"))));
	String msg_str(msg.getJsonString());
	

	return 0;	
}