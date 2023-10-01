#include <stdexcept>
#include <string>
#include <memory>

#include "gpio.hpp"
#include "bridge.hpp"
#include "registry.hpp"
#include "object.hpp"
#include "string.hpp"
#include "server_task.hpp"

#include "register_bridge_task.hpp"

using namespace task;
using namespace data;
using namespace hardware;

RegisterBridgeTask::RegisterBridgeTask(const std::string& id, const std::string& bridge_id, const PinNumber& left_shoulder_out, const PinNumber& right_shoulder_out, const Bridge::State& initial_state, BridgeRegistry& registry): ServerTask(id), m_bridge_id(bridge_id), m_left_shoulder_out(left_shoulder_out), m_right_shoulder_out(right_shoulder_out), m_initial_state(initial_state), m_registry(registry) {

}

void RegisterBridgeTask::execute() {
	if (m_registry.is_registered(m_bridge_id)) {
		m_report.insert({"result", std::shared_ptr<IData>(new String("FAIL"))});
		m_report.insert({"what", std::shared_ptr<IData>(new String("already registered"))});
		return;
	}
	std::shared_ptr<Bridge> bridge(new Bridge(m_left_shoulder_out, m_right_shoulder_out, m_initial_state));

	m_registry.register_member(m_bridge_id, bridge);
	m_report.insert({"result", std::shared_ptr<IData>(new String("OK"))});
}

data::Object RegisterBridgeTask::report() const {
	return m_report;
}