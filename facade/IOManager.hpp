#ifndef IO_MANAGER_HPP_INCLUDED
#define IO_MANAGER_HPP_INCLUDED

#include <map>
#include <functional>
#include <filesystem>
#include "Data.hpp"
#include "VoxelsCanvas.hpp"
#include "StackSegment.hpp"

#define SIZE_OF_EXTENSION 16

class Extension: public StackSegment<SIZE_OF_ACTION_KEY> {
public:
    Extension(): StackSegment<SIZE_OF_EXTENSION>(0){}
	Extension(const char* c): StackSegment<SIZE_OF_EXTENSION>(0){
        this->override(c, strlen(c));
    }
};

inline bool operator<(const Extension& k1, const Extension& k2) {
    return strcmp(k1.c_str(), k2.c_str()) < 0;
}

class DataProxy;

class IOManager {

    std::map<Extension, std::function<DataProxy*(const std::filesystem::path&)>> extensions;

public:

    std::unique_ptr<Data> open_from_disk(const std::filesystem::path& path);
    std::unique_ptr<Data> open_from_url(const std::string& url);

public:

    static IOManager& iomanager();
    static std::unique_ptr<IOManager> instance;
};

#endif //IO_MANAGER_HPP_INCLUDED