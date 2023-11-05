#ifndef DATA_PROXY_HPP_INCLUDED
#define DATA_PROXY_HPP_INCLUDED

#include <filesystem>
#include "Data.hpp"
#include "MetadataManager.hpp"

class DataProxy {
    
    std::filesystem::path data_path;

public:

    virtual void load_metadata() = 0;
    virtual Data* instanciate_data() = 0;

    DataProxy() = delete;
    DataProxy(const std::filesystem::path& p);
};

#endif //DATA_PROXY_HPP_INCLUDED