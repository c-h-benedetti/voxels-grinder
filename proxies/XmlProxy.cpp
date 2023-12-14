#include "XmlProxy.hpp"

void XmlProxy::load_metadata() {}

Data* XmlProxy::instanciate_data() {
    return nullptr;
}

XmlProxy::XmlProxy(const std::filesystem::path& p) : DataProxy(p) {}
