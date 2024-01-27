#ifndef	INVENTORY_ITEM_TASK_BUILDER_HPP
#define	INVENTORY_ITEM_TASK_BUILDER_HPP

#include "creator.hpp"
#include "data.hpp"
#include "inventory.hpp"
#include "task.hpp"
#include <memory>

namespace tasks {
	template <class Tkey, class Titem>
	class CreateInventoryItemTask: public basics::Task {
	public:
		CreateInventoryItemTask(
			inventory::Inventory<Tkey, Titem>& inventory, 
			const Tkey& key, 
			const data::Data& item_cfg, 
			const basics::Creator<Titem *, data::Data>& item_creator
		);
		CreateInventoryItemTask(const CreateInventoryItemTask& other) = default;
		CreateInventoryItemTask& operator=(const CreateInventoryItemTask& other) = default;

		virtual void execute() override;
	private:
		inventory::Inventory<Tkey, Titem>& m_inventory;
		Tkey m_key;
		std::unique_ptr<data::Data> m_item_cfg;
		basics::Creator<Titem *, data::Data>& m_item_creator;
	};

	template <class Tkey, class Titem>
	CreateInventoryItemTask<Tkey, Titem>::CreateInventoryItemTask(
		inventory::Inventory<Tkey, Titem>& inventory, 
		const Tkey& key, 
		const data::Data& item_cfg, 
		const basics::Creator<Titem *, data::Data>& item_creator
	): m_inventory(inventory), m_key(key), m_item_cfg(item_cfg.copy()), m_item_creator(std::ref(item_creator)) {

	}

	template <class Tkey, class Titem>
	void CreateInventoryItemTask<Tkey, Titem>::execute() {
		m_inventory.put(m_key, std::shared_ptr<int>(m_item_creator.create(*m_item_cfg)));
	}
}
#endif // INVENTORY_ITEM_TASK_BUILDER_HPP