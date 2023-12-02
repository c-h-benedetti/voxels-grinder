#ifndef OBJ_PROXY_HPP_INCLUDED
#define OBJ_PROXY_HPP_INCLUDED

#include "DataProxy.hpp"
#include "GeometryCanvas.hpp"

class ObjProxy : public DataProxy {
public:
    void load_metadata() override;
    Data* instanciate_data() override;

    ObjProxy() = delete;
    ObjProxy(const std::filesystem::path& p);
};

#endif //OBJ_PROXY_HPP_INCLUDED