#ifndef IO_MANAGER_HPP_INCLUDED
#define IO_MANAGER_HPP_INCLUDED

#include <map>
#include <functional>
#include <filesystem>
#include "core/Data.hpp"
#include "containers/VoxelsCanvas.hpp"
#include "stack-segments/StackSegment.hpp"

#include "proxies/TiffProxy.hpp"
#include "proxies/XmlProxy.hpp"
#include "proxies/ObjProxy.hpp"
#include "proxies/CziProxy.hpp"
#include "proxies/PgmProxy.hpp"

#define SIZE_OF_EXTENSION 16

class Extension: public StackSegment<SIZE_OF_EXTENSION> {
public:
    Extension(): StackSegment<SIZE_OF_EXTENSION>(0){}
	Extension(const char* c): StackSegment<SIZE_OF_EXTENSION>(0){
        this->override(c, strlen(c));
    }
};

inline bool operator<(const Extension& k1, const Extension& k2) {
    return strcmp(k1.c_str(), k2.c_str()) < 0;
}

class IOManager {

    std::map<Extension, std::function<DataProxy*(const std::filesystem::path&)>> extensions = {
        {".tif", [](const std::filesystem::path& p){ return new TiffProxy(p); }},
        {".tiff", [](const std::filesystem::path& p){ return new TiffProxy(p); }},
        {".czi", [](const std::filesystem::path& p){ return new CziProxy(p); }},
        {".obj", [](const std::filesystem::path& p){ return new ObjProxy(p); }},
        {".pgm", [](const std::filesystem::path& p){ return new PgmProxy(p); }},
        {".xml", [](const std::filesystem::path& p){ return new XmlProxy(p); }}
    };

public:

    std::unique_ptr<Data> open_from_disk(const std::filesystem::path& path);
    std::unique_ptr<Data> open_from_url(const std::string& url);
    bool is_image_path(const std::string& path);
    bool is_image_url(const std::string& url);

public:

    static IOManager& iomanager();
    static std::unique_ptr<IOManager> instance;
};

#endif //IO_MANAGER_HPP_INCLUDED