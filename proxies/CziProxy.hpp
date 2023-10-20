#ifndef CZI_PROXY_HPP_INCLUDED
#define CZI_PROXY_HPP_INCLUDED

#include "VoxelsCanvas.hpp"
#include "DataProxy.hpp"

class CziProxy : public DataProxy {
public:
    void load_metadata() override;
    Data* instanciate_data() override;

    CziProxy() = delete;
    CziProxy(const std::filesystem::path& p);
};

#endif //CZI_PROXY_HPP_INCLUDED