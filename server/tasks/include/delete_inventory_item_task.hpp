#ifndef	DELETE_INVENTORY_ITEM_TASK_HPP
#define	DELETE_INVENTORY_ITEM_TASK_HPP

#include "inventory.hpp"
#include "report.hpp"
#include "string.hpp"
#include "task.hpp"

namespace tasks {
	template <class Tkey, class Titem>
	class DeleteInventoryItemTask: public basics::Task<cnc_engine::Report> {
	public:
		DeleteInventoryItemTask(inventory::Inventory<Tkey, Titem>& inventory, const Tkey& key);
		DeleteInventoryItemTask(const DeleteInventoryItemTask& other) = default;
		DeleteInventoryItemTask& operator=(const DeleteInventoryItemTask& other) = default;

		virtual cnc_engine::Report execute() override;
	private:
		inventory::Inventory<Tkey, Titem>& m_inventory;
		Tkey m_key;
	};

	template <class Tkey, class Titem>
	DeleteInventoryItemTask<Tkey, Titem>::DeleteInventoryItemTask(inventory::Inventory<Tkey, Titem>& inventory, const Tkey& key): m_inventory(inventory), m_key(key) {

	}

	template <class Tkey, class Titem>
	cnc_engine::Report DeleteInventoryItemTask<Tkey, Titem>::execute() {
		cnc_engine::Report::Result result = cnc_engine::Report::Result::FAILURE;
		std::string report_data("");
		try {
			Titem *item_ptr = m_inventory.get(m_key);
			delete item_ptr;
			item_ptr = nullptr;
			result = cnc_engine::Report::Result::SUCCESS;
		} catch (const std::exception& e) {
			result = cnc_engine::Report::Result::FAILURE;
			report_data = e.what();
		}
		return cnc_engine::Report(result, cnc_engine::String(report_data));
	}
}
#endif // DELETE_INVENTORY_ITEM_TASK_HPP