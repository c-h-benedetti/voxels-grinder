#ifndef DATA_PROXY_HPP_INCLUDED
#define DATA_PROXY_HPP_INCLUDED

#include <filesystem>
#include "MetadataManager.hpp"

class Data;

class DataProxy {
    
    std::filesystem::path data_path;

public:

    virtual void load_metadata() = 0;
    virtual Data* instanciate_data() = 0;

    DataProxy() = delete;
    DataProxy(const std::filesystem::path& p);
};

#endif //DATA_PROXY_HPP_INCLUDED