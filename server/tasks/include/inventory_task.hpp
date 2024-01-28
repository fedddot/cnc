#ifndef	INVENTORY_TASK_HPP
#define	INVENTORY_TASK_HPP

#include <functional>
#include <memory>
#include <stdexcept>

#include "data.hpp"
#include "inventory.hpp"
#include "task.hpp"

namespace tasks {
	template <class Tkey, class Titem>
	class InventoryTask: public basics::Task {
	public:
		using InventoryAction = std::function<void(inventory::Inventory<Tkey, Titem> *, const data::Data&)>;
		InventoryTask(inventory::Inventory<Tkey, Titem> *inventory, const InventoryAction& inventory_action, const data::Data& task_data);
		InventoryTask(const InventoryTask& other) = delete;
		InventoryTask& operator=(const InventoryTask& other) = delete;

		virtual void execute() override;
	private:
		inventory::Inventory<Tkey, Titem> *m_inventory;
		InventoryAction m_inventory_action;
		std::unique_ptr<data::Data> m_task_data;
	};

	template <class Tkey, class Titem>
	InventoryTask<Tkey, Titem>::InventoryTask(inventory::Inventory<Tkey, Titem> *inventory, const InventoryAction& inventory_action, const data::Data& task_data): m_inventory(inventory), m_inventory_action(inventory_action), m_task_data(task_data.copy()) {
		if (!inventory) {
			throw std::invalid_argument("invalid inventory ptr received");
		}
		if (!inventory_action) {
			throw std::invalid_argument("invalid inventory action received");
		}
	}

	template <class Tkey, class Titem>
	void InventoryTask<Tkey, Titem>::execute() {
		m_inventory_action(m_inventory, *m_task_data);
	}
}
#endif // INVENTORY_TASK_HPP