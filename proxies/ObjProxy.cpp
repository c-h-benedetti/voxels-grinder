#include "ObjProxy.hpp"

void ObjProxy::load_metadata() {

}

Data* ObjProxy::instanciate_data() {
    return nullptr;
}

ObjProxy::ObjProxy(const std::filesystem::path& p) : DataProxy(p) {}
