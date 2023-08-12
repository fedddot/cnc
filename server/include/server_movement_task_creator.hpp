#ifndef	__SERVER_MOVEMENT_TASK_CREATOR_HPP__
#define	__SERVER_MOVEMENT_TASK_CREATOR_HPP__

#include <string>
#include <vector>

#include "itask.hpp"
#include "idata.hpp"
#include "isender.hpp"
#include "icreator.hpp"

namespace cnc {
	class MovementTaskCreator: public common::ICreator<std::shared_ptr<common::ITask>, const data::IData&> {
	public:
		MovementTaskCreator(communication::ISender<const std::vector<char>&>& sender, const std::string& distance_field_name = "distance", const std::string& speed_field_name = "speed", const std::string& axis_field_name = "axis");
		MovementTaskCreator(const MovementTaskCreator& other) = delete;
		MovementTaskCreator& operator=(const MovementTaskCreator& other) = delete;

		virtual std::shared_ptr<common::ITask> create(const data::IData& config_data) override;
	private:
		communication::ISender<const std::vector<char>&>& m_sender;

		const std::string m_distance_field_name;
		const std::string m_speed_field_name;
		const std::string m_axis_field_name;

		static std::string getStringDataField(const data::IData& data, const std::string& field_name);
	}; // MovementTaskCreator
} // namespace cnc

#endif // __SERVER_MOVEMENT_TASK_CREATOR_HPP__