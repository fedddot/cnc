#include <stdexcept>

#include "uart.hpp"
#include "package_manager.hpp"
#include "package_descriptor.hpp"

#include "server_package_manager.hpp"

using namespace communication;
using namespace hardware;

ServerPackageManager::ServerPackageManager(const PackageDescriptor& package_descriptor): PackageManager(package_descriptor, AAA) {
	
}