#ifndef	INVENTORY_ITEM_TASK_BUILDER_HPP
#define	INVENTORY_ITEM_TASK_BUILDER_HPP

#include "builder.hpp"
#include "inventory.hpp"
#include "task.hpp"
#include <stdexcept>

namespace tasks {
	template <class Tkey, class Titem>
	class InventoryItemTaskBuilder: public basics::Builder<basics::Task *> {
	public:
		InventoryItemTaskBuilder();
		InventoryItemTaskBuilder(const InventoryItemTaskBuilder& other) = default;
		InventoryItemTaskBuilder& operator=(const InventoryItemTaskBuilder& other) = default;

		void set_inventory(inventory::Inventory<Tkey, Titem> *inventory);
		inventory::Inventory<Tkey, Titem>& inventory() const;

		void set_key(const Tkey& key);
		Tkey key() const;
	private:
		mutable inventory::Inventory<Tkey, Titem> *m_inventory;
		Tkey m_key;
	};

	template <class Tkey, class Titem>
	inline InventoryItemTaskBuilder<Tkey, Titem>::InventoryItemTaskBuilder(): m_inventory(nullptr) {
	}

	template <class Tkey, class Titem>
	inline void InventoryItemTaskBuilder<Tkey, Titem>::set_inventory(inventory::Inventory<Tkey, Titem> *inventory) {
		m_inventory = inventory;
	}

	template <class Tkey, class Titem>
	inline inventory::Inventory<Tkey, Titem>& InventoryItemTaskBuilder<Tkey, Titem>::inventory() const {
		if (nullptr == m_inventory) {
			throw std::runtime_error("inventory has not been set");
		}
		return std::ref(*m_inventory);
	}

	template <class Tkey, class Titem>
	inline void InventoryItemTaskBuilder<Tkey, Titem>::set_key(const Tkey& key) {
		m_key = key;
	}

	template <class Tkey, class Titem>
	inline Tkey InventoryItemTaskBuilder<Tkey, Titem>::key() const {
		return m_key;
	}
}
#endif // INVENTORY_ITEM_TASK_BUILDER_HPP