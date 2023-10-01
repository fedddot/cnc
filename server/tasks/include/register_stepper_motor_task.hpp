#ifndef	__REGISTER_BRIDGE_TASK_HPP__
#define	__REGISTER_BRIDGE_TASK_HPP__

#include <string>
#include <memory>

#include "gpio.hpp"
#include "bridge.hpp"
#include "registry.hpp"
#include "object.hpp"
#include "server_task.hpp"

namespace task {
	class RegisterBridgeTask: public ServerTask {
	public:
		typedef hardware::Gpio::PinNumber PinNumber;
		typedef common::Registry<std::string, std::shared_ptr<hardware::Bridge>> BridgeRegistry;

		RegisterBridgeTask(const std::string& id, const std::string& bridge_id, const PinNumber& left_shoulder_out, const PinNumber& right_shoulder_out, const hardware::Bridge::State& initial_state, BridgeRegistry& registry);
		RegisterBridgeTask(const RegisterBridgeTask& other) = delete;
		RegisterBridgeTask& operator=(const RegisterBridgeTask& other) = delete;
		
		virtual void execute() override;
		virtual data::Object report() const override;
	private:
		std::string m_bridge_id;
		PinNumber m_left_shoulder_out;
		PinNumber m_right_shoulder_out;
		hardware::Bridge::State m_initial_state;
		BridgeRegistry& m_registry;
		data::Object m_report;
	}; // RegisterBridgeTask
} // namespace task

#endif // __REGISTER_BRIDGE_TASK_HPP__