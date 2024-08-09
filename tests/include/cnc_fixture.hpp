#ifndef CNC_FIXTURE_HPP
#define CNC_FIXTURE_HPP

#include <memory>
#include <string>

#include "data.hpp"
#include "ipc_connection.hpp"
#include "linux_ipc_connection.hpp"
#include "mcu_client.hpp"
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
			m_connection = std::unique_ptr<mcu_ipc::IpcConnection<std::string>>(
				new linux_mcu_ipc::LinuxIpcConnection(
					"/dev/ttyACM0",
					linux_mcu_ipc::LinuxIpcConnection::Baud::BAUD115200,
					"MSG_HEADER",
					"MSG_TAIL",
					1000UL
				)
			);
		}
		void TearDown() override {
			m_server = nullptr;
			m_connection = nullptr;
		}

		mcu_server::Data *run_data(const mcu_server::Data& data) const {
			auto serial_data = serializer().serialize(data);
			std::cout << std::endl << std::endl << "running data:" << std::endl << '\t' << serial_data << std::endl;
			auto serial_report = m_server->run(serial_data);
			std::cout << std::endl << "received report:" << std::endl << '\t' << serial_report << std::endl;
			return parser().parse(serial_report);
		}

		mcu_server::Data *run_data_on_mcu(const mcu_server::Data& data) const {
			auto serial_data = mcu_server_utl::JsonDataSerializer().serialize(data);
			std::cout << std::endl << std::endl << "running data:" << std::endl << '\t' << serial_data << std::endl;
			mcu_client::McuClient<std::string> client(m_connection.get());
			auto serial_report = client.run(serial_data);
			std::cout << std::endl << "received report:" << std::endl << '\t' << serial_report << std::endl;
			return parser().parse(serial_report);
		}
	private:
		std::unique_ptr<mcu_server::McuServer<std::string>> m_server;
		std::unique_ptr<mcu_ipc::IpcConnection<std::string>> m_connection;
	};
}

#endif