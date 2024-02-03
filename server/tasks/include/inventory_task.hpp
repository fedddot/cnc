#ifndef	INVENTORY_TASK_HPP
#define	INVENTORY_TASK_HPP

#include <exception>
#include <functional>
#include <memory>
#include <stdexcept>

#include "data.hpp"
#include "integer.hpp"
#include "inventory.hpp"
#include "object.hpp"
#include "string.hpp"
#include "task.hpp"

namespace tasks {
	template <class Tkey, class Titem>
	class InventoryTask: public engine::Task {
	public:
		using InventoryAction = std::function<void(inventory::Inventory<Tkey, Titem> *, const data::Data&)>;
		InventoryTask(inventory::Inventory<Tkey, Titem> *inventory, const InventoryAction& inventory_action, const data::Data& task_data);
		InventoryTask(const InventoryTask& other) = delete;
		InventoryTask& operator=(const InventoryTask& other) = delete;

		virtual void execute() override;
		virtual const data::Data& report() const override;
	private:
		inventory::Inventory<Tkey, Titem> *m_inventory;
		InventoryAction m_inventory_action;
		std::unique_ptr<data::Data> m_task_data;
		data::Object m_report_data;
	};

	template <class Tkey, class Titem>
	inline InventoryTask<Tkey, Titem>::InventoryTask(inventory::Inventory<Tkey, Titem> *inventory, const InventoryAction& inventory_action, const data::Data& task_data): m_inventory(inventory), m_inventory_action(inventory_action), m_task_data(task_data.clone()) {
		if (!inventory) {
			throw std::invalid_argument("invalid inventory ptr received");
		}
		if (!inventory_action) {
			throw std::invalid_argument("invalid inventory action received");
		}
	}

	template <class Tkey, class Titem>
	inline void InventoryTask<Tkey, Titem>::execute() {
		try {
			m_inventory_action(m_inventory, *m_task_data);
			m_report_data.add("result", data::Integer(0));
		} catch (const std::exception& e) {
			m_report_data.add("result", data::Integer(-1));
			m_report_data.add("exception", data::String(e.what()));
		}
	}

	template <class Tkey, class Titem>
	inline const data::Data& InventoryTask<Tkey, Titem>::report() const {
		return std::ref(m_report_data);
	}
}
#endif // INVENTORY_TASK_HPP