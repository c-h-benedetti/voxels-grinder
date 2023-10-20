#ifndef TIFF_PROXY_HPP_INCLUDED
#define TIFF_PROXY_HPP_INCLUDED

#include "DataProxy.hpp"
#include "VoxelsCanvas.hpp"

class TiffProxy : public DataProxy {
public:
    void load_metadata() override;
    Data* instanciate_data() override;

    TiffProxy() = delete;
    TiffProxy(const std::filesystem::path& p);
};

#endif //TIFF_PROXY_HPP_INCLUDED