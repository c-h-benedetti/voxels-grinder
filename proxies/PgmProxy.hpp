#ifndef PGM_PROXY_HPP_INCLUDED
#define PGM_PROXY_HPP_INCLUDED

#include "DataProxy.hpp"
#include "VoxelsCanvas.hpp"

class PgmProxy : public DataProxy {
public:
    void load_metadata() override;
    Data* instanciate_data() override;

    PgmProxy() = delete;
    PgmProxy(const std::filesystem::path& p);
};

#endif //PGM_PROXY_HPP_INCLUDED