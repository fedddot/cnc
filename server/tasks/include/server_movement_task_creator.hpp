#ifndef	__SERVER_MOVEMENT_TASK_CREATOR_HPP__
#define	__SERVER_MOVEMENT_TASK_CREATOR_HPP__

#include <string>
#include <vector>

#include "iserver_task.hpp"
#include "idata.hpp"
#include "icreator.hpp"

namespace task {
	class MovementTaskCreator: public common::ICreator<std::shared_ptr<IServerTask>, data::IData> {
	public:
		MovementTaskCreator(const std::string& distance_field_name = "distance", const std::string& speed_field_name = "speed", const std::string& axis_field_name = "axis");
		MovementTaskCreator(const MovementTaskCreator& other) = delete;
		MovementTaskCreator& operator=(const MovementTaskCreator& other) = delete;

		virtual std::shared_ptr<IServerTask> create(const data::IData& config_data) override;
	private:
		const std::string m_distance_field_name;
		const std::string m_speed_field_name;
		const std::string m_axis_field_name;

		static std::string getStringDataField(const data::IData& data, const std::string& field_name);
	}; // MovementTaskCreator
} // namespace cnc

#endif // __SERVER_MOVEMENT_TASK_CREATOR_HPP__