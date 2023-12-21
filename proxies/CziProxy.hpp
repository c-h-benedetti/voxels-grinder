#ifndef CZI_PROXY_HPP_INCLUDED
#define CZI_PROXY_HPP_INCLUDED

#include "core/DataProxy.hpp"
#include "containers/VoxelsCanvas.hpp"

class CziProxy : public DataProxy {
public:
    void load_metadata() override;
    Data* instanciate_data() override;

    CziProxy() = delete;
    CziProxy(const std::filesystem::path& p);
};

#endif //CZI_PROXY_HPP_INCLUDED