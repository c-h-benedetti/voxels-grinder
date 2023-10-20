#include "CziProxy.hpp"

void CziProxy::load_metadata() {

}

Data* CziProxy::instanciate_data() {
    return nullptr;
}

CziProxy::CziProxy(const std::filesystem::path& p) : DataProxy(p) {}