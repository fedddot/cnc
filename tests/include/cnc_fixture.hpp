#ifndef CNC_FIXTURE_HPP
#define CNC_FIXTURE_HPP

#include <memory>
#include <string>

#include "gtest/gtest.h"

#include "data.hpp"
#include "gpo.hpp"
#include "ipc_connection.hpp"
#include "json_data_serializer.hpp"
#include "linux_ipc_connection.hpp"
#include "mcu_client.hpp"
#include "mcu_server.hpp"
#include "remote_stepper_motor.hpp"

namespace cnc_uts {
	class CncFixture: public testing::Test {
	public:
		using Shoulder = cnc::RemoteStepperMotor::Shoulder;
		using Shoulders = cnc::RemoteStepperMotor::Shoulders;
		using State = cnc::RemoteStepperMotor::State;
		using States = cnc::RemoteStepperMotor::States;
		using GpoState = mcu_platform::Gpo::State;
		using StepperId = int;
		using Direction = cnc::RemoteStepperMotor::Direction;

		CncFixture() = default;
		CncFixture(const CncFixture& other) = delete;
		CncFixture& operator=(const CncFixture& other) = delete;

		void SetUp() override {
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

		void run_data(const mcu_server::Data& data) const {
			auto serial_data = mcu_server_utl::JsonDataSerializer().serialize(data);
			std::cout << std::endl << std::endl << "running data:" << std::endl << '\t' << serial_data << std::endl;
			auto serial_report = m_server->run(serial_data);
			std::cout << std::endl << "received report:" << std::endl << '\t' << serial_report << std::endl;
		}

		void run_data_on_mcu(const mcu_server::Data& data) const {
			auto serial_data = mcu_server_utl::JsonDataSerializer().serialize(data);
			std::cout << std::endl << std::endl << "running data:" << std::endl << '\t' << serial_data << std::endl;
			mcu_client::McuClient<std::string> client(m_connection.get());
			auto serial_report = client.run(serial_data);
			std::cout << std::endl << "received report:" << std::endl << '\t' << serial_report << std::endl;
		}

		States states() const {
			return States {
				{
					{Shoulder::IN0, GpoState::HIGH},
					{Shoulder::IN1, GpoState::LOW},
					{Shoulder::IN2, GpoState::LOW},
					{Shoulder::IN3, GpoState::LOW}
				},
				{
					{Shoulder::IN0, GpoState::HIGH},
					{Shoulder::IN1, GpoState::LOW},
					{Shoulder::IN2, GpoState::HIGH},
					{Shoulder::IN3, GpoState::LOW}
				},
				{
					{Shoulder::IN0, GpoState::LOW},
					{Shoulder::IN1, GpoState::LOW},
					{Shoulder::IN2, GpoState::HIGH},
					{Shoulder::IN3, GpoState::LOW}
				},
				{
					{Shoulder::IN0, GpoState::LOW},
					{Shoulder::IN1, GpoState::HIGH},
					{Shoulder::IN2, GpoState::HIGH},
					{Shoulder::IN3, GpoState::LOW}
				},
				{
					{Shoulder::IN0, GpoState::LOW},
					{Shoulder::IN1, GpoState::HIGH},
					{Shoulder::IN2, GpoState::LOW},
					{Shoulder::IN3, GpoState::LOW}
				},
				{
					{Shoulder::IN0, GpoState::LOW},
					{Shoulder::IN1, GpoState::HIGH},
					{Shoulder::IN2, GpoState::LOW},
					{Shoulder::IN3, GpoState::HIGH}
				},
				{
					{Shoulder::IN0, GpoState::LOW},
					{Shoulder::IN1, GpoState::LOW},
					{Shoulder::IN2, GpoState::LOW},
					{Shoulder::IN3, GpoState::HIGH}
				},
				{
					{Shoulder::IN0, GpoState::HIGH},
					{Shoulder::IN1, GpoState::LOW},
					{Shoulder::IN2, GpoState::LOW},
					{Shoulder::IN3, GpoState::HIGH}
				}
			};
		}
	private:
		std::unique_ptr<mcu_server::McuServer<std::string>> m_server;
		std::unique_ptr<mcu_ipc::IpcConnection<std::string>> m_connection;
	};
}

#endif