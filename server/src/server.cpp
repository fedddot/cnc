#include "create_inventory_item_task.hpp"
#include "creator.hpp"
#include "data.hpp"
#include "engine.hpp"
#include "integer.hpp"
#include "inventory.hpp"
#include "object.hpp"
#include "report.hpp"
#include "sender.hpp"
#include "string.hpp"
#include "task_factory.hpp"
#include <iostream>
#include <map>
#include <string>

using namespace inventory;
using namespace cnc_engine;
using namespace basics;
using namespace task_factory;
using namespace tasks;

class ServerSender: public Engine::ReportSender {
public:
	virtual void send(const Report& data) const {
		std::cout << "sending report ..." << std::endl;
		std::cout << "\tresult = " << static_cast<int>(data.result()) << std::endl;
	}
};

class StringItemCreator: public Creator<std::string *, TaskFactory::TaskConfigData> {
public:
	virtual inline std::string *create(const TaskFactory::TaskConfigData& cfg) const override {
		return new std::string(Data::cast<String>(Data::cast<Object>(cfg).access("item_val")).get());
	}
};

class ServerTaskCreator: public TaskFactory::TaskCreator {
public:
	inline ServerTaskCreator(Inventory<int, std::string>& string_inv): m_string_inv(string_inv) {}
	
	virtual inline TaskFactory::Task *create(const TaskFactory::TaskConfigData& task_config) const override {
		const Object& cfg_obj = Data::cast<Object>(task_config);
		const StringItemCreator creator;
		return new CreateInventoryItemTask<int, std::string>(
			m_string_inv,
			Data::cast<Integer>(cfg_obj.access("key")).get(),
			task_config,
			creator
		);
	}
private:
	Inventory<int, std::string>& m_string_inv;
};

int main(void) {
	Inventory<int, std::string> strings_inventory;
	ServerTaskCreator creator(strings_inventory);
	TaskFactory factory(
		std::map<TaskFactory::TaskType, TaskFactory::TaskCreator&> {
			{ TaskFactory::TaskType::CREATE_INVENTORY_ITEM, std::ref(creator) }
		}
	);
	ServerSender sender;

	Engine engine(factory, sender);
}