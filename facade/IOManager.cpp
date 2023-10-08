#include "IOManager.hpp"

IOManager& IOManager::iomanager() { return *IOManager::instance; }

std::unique_ptr<IOManager> IOManager::instance = std::make_unique<IOManager>();