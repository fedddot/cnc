#ifndef	CREATE_INVENTORY_ITEM_TASK_BUILDER_HPP
#define	CREATE_INVENTORY_ITEM_TASK_BUILDER_HPP

#include "creator.hpp"
#include "data.hpp"
#include "inventory_item_task_builder.hpp"
#include "inventory.hpp"
#include "task.hpp"
#include <memory>
#include <stdexcept>

namespace tasks {
	template <class Tkey, class Titem>
	class CreateInventoryItemTaskBuilder: public InventoryItemTaskBuilder<Tkey, Titem> {
	public:
		CreateInventoryItemTaskBuilder();
		CreateInventoryItemTaskBuilder(const CreateInventoryItemTaskBuilder& other);
		CreateInventoryItemTaskBuilder& operator=(const CreateInventoryItemTaskBuilder& other);

		void set_item_cfg_data(const data::Data& data);
		void set_item_creator(const basics::Creator<Titem *, data::Data> *item_creator);
		Tkey key() const;
	private:
		std::unique_ptr<data::Data> m_data;
		const basics::Creator<Titem *, data::Data> *m_item_creator;
	};

	template <class Tkey, class Titem>
	inline CreateInventoryItemTaskBuilder<Tkey, Titem>::CreateInventoryItemTaskBuilder(): m_data(nullptr), m_item_creator(nullptr) {
	}

	template <class Tkey, class Titem>
	inline void CreateInventoryItemTaskBuilder<Tkey, Titem>::set_item_cfg_data(const data::Data& data) {
		m_data = std::unique_ptr<data::Data>(data.copy());
	}

	template <class Tkey, class Titem>
	inline inventory::Inventory<Tkey, Titem>& CreateInventoryItemTaskBuilder<Tkey, Titem>::inventory() const {
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
#endif // CREATE_INVENTORY_ITEM_TASK_BUILDER_HPP