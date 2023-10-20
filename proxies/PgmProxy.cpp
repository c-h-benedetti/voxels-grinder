#include "PgmProxy.hpp"

void PgmProxy::load_metadata() {}

Data* PgmProxy::instanciate_data() {
    return nullptr;
}

PgmProxy::PgmProxy(const std::filesystem::path& p) : DataProxy(p) {}
