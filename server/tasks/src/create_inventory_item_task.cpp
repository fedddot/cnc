#include "create_inventory_item_task.hpp"
#include "report.hpp"
#include "string.hpp"
#include <exception>
using namespace basics;
using namespace tasks;
using namespace cnc_engine;
using namespace inventory;

template <class Tkey, class Titem, class Tconf>
CreateInventoryItemTask<Tkey, Titem, Tconf>::CreateInventoryItemTask(Inventory<Tkey, Titem>& inventory, const Tkey& key, const Tconf& item_cfg, const Creator<Titem *, Tconf>& item_creator): m_inventory(inventory), m_key(key), m_item_cfg(item_cfg), m_item_creator(item_creator) {

}

template <class Tkey, class Titem, class Tconf>
Report CreateInventoryItemTask<Tkey, Titem, Tconf>::execute() {
	Report::Result result = Report::Result::FAILURE;
	std::string report_data("");
	try {
		m_inventory.put(m_key, m_item_creator(m_item_cfg));
		result = Report::Result::SUCCESS;
	} catch (const std::exception& e) {
		result = Report::Result::FAILURE;
		report_data = e.what();
	}
	return Report(result, String(report_data));
}