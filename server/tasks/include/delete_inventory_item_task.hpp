#ifndef	DELETE_INVENTORY_ITEM_TASK_HPP
#define	DELETE_INVENTORY_ITEM_TASK_HPP

#include "inventory.hpp"
#include "task.hpp"

namespace tasks {
	template <class Tkey, class Titem>
	class DeleteInventoryItemTask: public basics::Task {
	public:
		DeleteInventoryItemTask(inventory::Inventory<Tkey, Titem>& inventory, const Tkey& key);
		DeleteInventoryItemTask(const DeleteInventoryItemTask& other) = default;
		DeleteInventoryItemTask& operator=(const DeleteInventoryItemTask& other) = default;

		virtual void execute() override;
	private:
		inventory::Inventory<Tkey, Titem>& m_inventory;
		Tkey m_key;
	};

	template <class Tkey, class Titem>
	DeleteInventoryItemTask<Tkey, Titem>::DeleteInventoryItemTask(inventory::Inventory<Tkey, Titem>& inventory, const Tkey& key): m_inventory(inventory), m_key(key) {

	}

	template <class Tkey, class Titem>
	void DeleteInventoryItemTask<Tkey, Titem>::execute() {
		m_inventory.remove(m_key);
	}
}
#endif // DELETE_INVENTORY_ITEM_TASK_HPP