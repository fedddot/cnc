#ifndef	INVENTORY_TASK_FACTORY_HPP
#define	INVENTORY_TASK_FACTORY_HPP

#include <functional>
#include <map>
#include <stdexcept>

#include "data.hpp"
#include "factory.hpp"
#include "inventory.hpp"
#include "task.hpp"

namespace task_factory {
	template <class Tkey, class Titem>
	class InventoryTaskFactory: public basics::Factory<basics::Task *, data::Data> {
	public:
		enum class TaskType: int {
			CREATE_INVENTORY_ITEM,
			DELETE_INVENTORY_ITEM,
			USE_INVENTORY_ITEM
		};
		using TaskCreator = std::function<basics::Task *(inventory::Inventory<Tkey, Titem> *inventory, const data::Data&)>;
		using TaskTypeRetriever = std::function<TaskType(const data::Data&)>;

		InventoryTaskFactory(inventory::Inventory<Tkey, Titem> *inventory, const TaskTypeRetriever& task_type_retriever);
		InventoryTaskFactory(const InventoryTaskFactory& other) = default;
		InventoryTaskFactory& operator=(const InventoryTaskFactory& other) = default;

		void set_creator(const TaskType& type, const TaskCreator& creator);
		void remove_creator(const TaskType& type);
		
		virtual basics::Task *create(const data::Data& cfg) const override;
	private:
		inventory::Inventory<Tkey, Titem> *m_inventory;
		TaskTypeRetriever m_task_type_retriever;
		std::map<TaskType, TaskCreator> m_creators;
	};

	template <class Tkey, class Titem>
	inline InventoryTaskFactory<Tkey, Titem>::InventoryTaskFactory(inventory::Inventory<Tkey, Titem> *inventory, const TaskTypeRetriever& task_type_retriever): m_inventory(inventory), m_task_type_retriever(task_type_retriever) {
		if (!m_inventory) {
			throw std::invalid_argument("invalid inventory ptr received");
		}
		if (!m_task_type_retriever) {
			throw std::invalid_argument("invalid task type retriever received");
		}
	}

	template <class Tkey, class Titem>
	inline void InventoryTaskFactory<Tkey, Titem>::set_creator(const TaskType& type, const TaskCreator& creator) {
		m_creators[type] = creator;
	}

	template <class Tkey, class Titem>
	inline void InventoryTaskFactory<Tkey, Titem>::remove_creator(const TaskType& type) {
		auto iter = m_creators.find(type);
		if (m_creators.end() == iter) {
			throw std::invalid_argument("creator with received type not found");
		}
		m_creators.erase(iter);
	}

	template <class Tkey, class Titem>
	inline basics::Task *InventoryTaskFactory<Tkey, Titem>::create(const data::Data& cfg) const {
		auto iter = m_creators.find(m_task_type_retriever(cfg));
		if (m_creators.end() == iter) {
			throw std::invalid_argument("creator matching received cfg is not registered");
		}
		return (iter->second)(m_inventory, cfg);
	}
}
#endif // INVENTORY_TASK_FACTORY_HPP