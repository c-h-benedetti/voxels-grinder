#include "TiffProxy.hpp"

void TiffProxy::load_metadata() {}

Data* TiffProxy::instanciate_data() {
    return nullptr;
}

TiffProxy::TiffProxy(const std::filesystem::path& p) : DataProxy(p) {}
