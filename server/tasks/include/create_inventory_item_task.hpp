#ifndef	CREATE_INVENTORY_ITEM_TASK_HPP
#define	CREATE_INVENTORY_ITEM_TASK_HPP

#include "creator.hpp"
#include "inventory.hpp"
#include "report.hpp"
#include "task.hpp"

namespace tasks {
	template <class Tkey, class Titem, class Tconf>
	class CreateInventoryItemTask: public basics::Task<cnc_engine::Report> {
	public:
		CreateInventoryItemTask(inventory::Inventory<Tkey, Titem>& inventory, const Tkey& key, const Tconf& item_cfg, const basics::Creator<Titem *, Tconf>& item_creator);
		CreateInventoryItemTask(const CreateInventoryItemTask& other) = default;
		CreateInventoryItemTask& operator=(const CreateInventoryItemTask& other) = default;

		virtual cnc_engine::Report execute() override;
	private:
		inventory::Inventory<Tkey, Titem>& m_inventory;
		Tkey m_key;
		Tconf m_item_cfg;
		const basics::Creator<Titem *, Tconf>& m_item_creator;
	};
}
#endif // CREATE_INVENTORY_ITEM_TASK_HPP