#ifndef	TASK_FACTORY_HPP
#define	TASK_FACTORY_HPP

#include <functional>
#include <map>
#include <stdexcept>
#include <string>

#include "data.hpp"
#include "factory.hpp"
#include "task.hpp"

namespace task_factory {
	class TaskFactory: public engine::Factory {
	public:
		using TaskCreator = std::function<engine::Task *(const data::Data&)>;
		using CreatorIdRetriever = std::function<std::string(const data::Data&)>;

		TaskFactory(const CreatorIdRetriever& id_retriever);
		TaskFactory(const TaskFactory& other) = default;
		TaskFactory& operator=(const TaskFactory& other) = default;

		virtual engine::Task *create(const data::Data& cfg) const override;
		void set_creator(const std::string& id, const TaskCreator& creator);
		void remove_creator(const std::string& id);
	private:
		CreatorIdRetriever m_id_retriever;
		std::map<std::string, TaskCreator> m_creators;
	};

	inline TaskFactory::TaskFactory(const CreatorIdRetriever& id_retriever): m_id_retriever(id_retriever) {

	}

	inline void TaskFactory::set_creator(const std::string& id, const TaskCreator& creator) {
		m_creators[id] = creator;
	}

	inline void TaskFactory::remove_creator(const std::string& id) {
		auto iter = m_creators.find(id);
		if (m_creators.end() == iter) {
			throw std::invalid_argument("creator with received type not found");
		}
		m_creators.erase(iter);
	}

	inline engine::Task *TaskFactory::create(const data::Data& cfg) const {
		auto iter = m_creators.find(m_id_retriever(cfg));
		if (m_creators.end() == iter) {
			throw std::invalid_argument("creator with received type not found");
		}
		return iter->second(cfg);
	}
}
#endif // TASK_FACTORY_HPP