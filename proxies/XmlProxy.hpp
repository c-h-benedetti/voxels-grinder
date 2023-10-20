#ifndef XML_PROXY_HPP_INCLUDED
#define XML_PROXY_HPP_INCLUDED

#include "DataProxy.hpp"
#include "VoxelsCanvas.hpp"

class XmlProxy : public DataProxy {
public:
    void load_metadata() override;
    Data* instanciate_data() override;

    XmlProxy() = delete;
    XmlProxy(const std::filesystem::path& p);
};

#endif //XML_PROXY_HPP_INCLUDED