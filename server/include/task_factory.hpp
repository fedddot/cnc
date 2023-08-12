#ifndef	__TASK_FACTORY_HPP__
#define	__TASK_FACTORY_HPP__

#include <string>
#include <vector>
#include <map>
#include <memory>

#include "idata.hpp"
#include "itask.hpp"
#include "icreator.hpp"

namespace cnc {
	class TaskFactory {
	public:
		typedef common::ICreator<std::shared_ptr<common::ITask>, const data::IData&> ITaskCreator;
		
		TaskFactory(const std::string& type_field_name = "type");
		TaskFactory(const TaskFactory& other) = default;
		TaskFactory& operator=(const TaskFactory& other) = default;
		~TaskFactory() noexcept = default;

		std::shared_ptr<common::ITask> create(const data::IData& config_data);
		void register_creator(const std::string& task_type, const std::shared_ptr<ITaskCreator>& creator);
	private:
		const std::string m_type_field_name;
		std::map<std::string, std::shared_ptr<ITaskCreator>> m_creators;

		std::string getType(const data::IData& config_data) const;
	}; // TaskFactory
} // namespace data
#endif // __TASK_FACTORY_HPP__