#ifndef	CREATE_INVENTORY_ITEM_TASK_HPP
#define	CREATE_INVENTORY_ITEM_TASK_HPP

#include <functional>
#include <memory>

#include "data.hpp"
#include "inventory.hpp"
#include "task.hpp"

namespace tasks {
	template <class Tkey, class Titem>
	class CreateInventoryItemTask: public basics::Task {
	public:
		using ItemCreator = std::function<Titem *(const data::Data&)>;
		CreateInventoryItemTask(
			inventory::Inventory<Tkey, Titem>& inventory, 
			const Tkey& key, 
			const data::Data& item_cfg, 
			const ItemCreator& item_creator
		);
		CreateInventoryItemTask(const CreateInventoryItemTask& other) = default;
		CreateInventoryItemTask& operator=(const CreateInventoryItemTask& other) = default;

		virtual void execute() override;
	private:
		inventory::Inventory<Tkey, Titem>& m_inventory;
		Tkey m_key;
		std::unique_ptr<data::Data> m_item_cfg;
		const ItemCreator& m_item_creator;
	};

	template <class Tkey, class Titem>
	CreateInventoryItemTask<Tkey, Titem>::CreateInventoryItemTask(
		inventory::Inventory<Tkey, Titem>& inventory, 
		const Tkey& key, 
		const data::Data& item_cfg, 
		const ItemCreator& item_creator
	): m_inventory(inventory), m_key(key), m_item_cfg(item_cfg.copy()), m_item_creator(item_creator) {

	}

	template <class Tkey, class Titem>
	void CreateInventoryItemTask<Tkey, Titem>::execute() {
		m_inventory.put(m_key, std::shared_ptr<Titem>(m_item_creator(*m_item_cfg)));
	}
}
#endif // CREATE_INVENTORY_ITEM_TASK_HPP