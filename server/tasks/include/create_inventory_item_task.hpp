#ifndef	CREATE_INVENTORY_ITEM_TASK_HPP
#define	CREATE_INVENTORY_ITEM_TASK_HPP

#include "creator.hpp"
#include "data.hpp"
#include "inventory.hpp"
#include "report.hpp"
#include "string.hpp"
#include "task.hpp"
#include <memory>

namespace tasks {
	template <class Tkey, class Titem>
	class CreateInventoryItemTask: public basics::Task<cnc_engine::Report> {
	public:
		CreateInventoryItemTask(inventory::Inventory<Tkey, Titem>& inventory, const Tkey& key, const cnc_engine::Data& item_cfg, basics::Creator<Titem *, cnc_engine::Data>& item_creator);
		CreateInventoryItemTask(const CreateInventoryItemTask& other) = default;
		CreateInventoryItemTask& operator=(const CreateInventoryItemTask& other) = default;

		virtual cnc_engine::Report execute() override;
	private:
		inventory::Inventory<Tkey, Titem>& m_inventory;
		Tkey m_key;
		std::unique_ptr<cnc_engine::Data> m_item_cfg;
		basics::Creator<Titem *, cnc_engine::Data>& m_item_creator;
	};

	template <class Tkey, class Titem>
	CreateInventoryItemTask<Tkey, Titem>::CreateInventoryItemTask(inventory::Inventory<Tkey, Titem>& inventory, const Tkey& key, const cnc_engine::Data& item_cfg, basics::Creator<Titem *, cnc_engine::Data>& item_creator): m_inventory(inventory), m_key(key), m_item_cfg(item_cfg.copy()), m_item_creator(std::ref(item_creator)) {

	}

	template <class Tkey, class Titem>
	cnc_engine::Report CreateInventoryItemTask<Tkey, Titem>::execute() {
		cnc_engine::Report::Result result = cnc_engine::Report::Result::FAILURE;
		std::string report_data("");
		try {
			m_inventory.put(m_key, m_item_creator.create(*m_item_cfg));
			result = cnc_engine::Report::Result::SUCCESS;
		} catch (const std::exception& e) {
			result = cnc_engine::Report::Result::FAILURE;
			report_data = e.what();
		}
		return cnc_engine::Report(result, cnc_engine::String(report_data));
	}
}
#endif // CREATE_INVENTORY_ITEM_TASK_HPP