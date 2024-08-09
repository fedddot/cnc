#ifndef CNC_FIXTURE_HPP
#define CNC_FIXTURE_HPP

#include <memory>
#include <string>

#include "data.hpp"
#include "mcu_server_fixture.hpp"
#include "mcu_server.hpp"

namespace cnc_uts {
	class CncFixture: public mcu_server_uts::McuServerFixture {
	public:
		CncFixture() = default;
		CncFixture(const CncFixture& other) = delete;
		CncFixture& operator=(const CncFixture& other) = delete;

		void SetUp() override {
			m_server = std::make_unique<mcu_server::McuServer<std::string>>(
				parser(),
				serializer(),
				factory(),
				fail_report_creator()
			);
		}
		void TearDown() override {
			m_server = nullptr;
		}

		mcu_server::Data *run_data(const mcu_server::Data& data) const {
			auto serial_data = serializer().serialize(data);
			std::cout << std::endl << std::endl << "running data:" << std::endl << serial_data << std::endl;
			auto serial_report = m_server->run(serial_data);
			std::cout << "received report:" << std::endl << serial_report << std::endl;
			return parser().parse(serial_report);
		}
	private:
		std::unique_ptr<mcu_server::McuServer<std::string>> m_server;
	};
}

#endif