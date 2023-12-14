#include "IOManager.hpp"

std::unique_ptr<IOManager> IOManager::instance = std::make_unique<IOManager>();

std::unique_ptr<Data> IOManager::open_from_disk(const std::filesystem::path& path) {
    // We need the target to be a file.
    if (!std::filesystem::is_regular_file(path)) { return nullptr; }
    // We need the target to have an extension.
    if (!path.has_extension()) { return nullptr; }
    // We isolate the extension and check that we are able to open it.
    Extension ext(path.extension().c_str());
    auto found = this->extensions.find(ext);
    // We abort if we can't open that type of file.
    if (found == this->extensions.end()) { return nullptr; }
    // We generate a new Data object that contains the new proxy.
    std::unique_ptr<DataProxy> proxy = std::unique_ptr<DataProxy>(found->second(path));
    std::unique_ptr<Data> data{proxy->instanciate_data()};
    return data;
}

std::unique_ptr<Data> IOManager::open_from_url(const std::string& url) {
    // Not implemented yet...
    return nullptr;
}

bool IOManager::is_image_path(const std::string& path) {
    return false;
}

bool IOManager::is_image_url(const std::string& url) {
    return false;
}