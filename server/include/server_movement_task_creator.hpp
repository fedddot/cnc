#ifndef	__SERVER_MOVEMENT_TASK_CREATOR_HPP__
#define	__SERVER_MOVEMENT_TASK_CREATOR_HPP__

#include <string>
#include <vector>

#include "itask.hpp"
#include "idata.hpp"
#include "idata_sender.hpp"
#include "task_factory.hpp"

namespace cnc {
	class MovementTaskCreator: public TaskFactory::ITaskCreator {
	public:
		MovementTaskCreator(data::IDataSender<const std::vector<char>&> *sender, const std::string& distance_field_name = "distance", const std::string& speed_field_name = "speed", const std::string& axis_field_name = "axis");
		MovementTaskCreator(const MovementTaskCreator& other) = default;
		MovementTaskCreator& operator=(const MovementTaskCreator& other) = default;
		~MovementTaskCreator() noexcept = default;

		virtual std::shared_ptr<common::ITask> create(const data::IData& config_data) override;
	private:
		data::IDataSender<const std::vector<char>&> *m_sender;

		const std::string m_distance_field_name;
		const std::string m_speed_field_name;
		const std::string m_axis_field_name;

		static data::IDataSender<const std::vector<char>&> *init_sender(data::IDataSender<const std::vector<char>&> *sender);
		static std::string getStringDataField(const data::IData& data, const std::string& field_name);
	}; // MovementTaskCreator
} // namespace cnc

#endif // __SERVER_MOVEMENT_TASK_CREATOR_HPP__