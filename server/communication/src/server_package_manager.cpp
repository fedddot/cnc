#include <stdexcept>

#include "package_manager.hpp"
#include "package_descriptor.hpp"
#include "server_uart.hpp"

#include "server_package_manager.hpp"

using namespace communication;
using namespace hardware;

ServerPackageManager::ServerPackageManager(const PackageDescriptor& package_descriptor, ServerUart& uart): PackageManager(package_descriptor, uart), m_uart(uart) {
	m_uart.set_char_listener(&(receiver()));
}

ServerPackageManager::~ServerPackageManager() noexcept {
	m_uart.set_char_listener(nullptr);
}