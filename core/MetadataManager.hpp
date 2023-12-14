#ifndef META_DATA_MANAGER_HPP_INCLUDED
#define META_DATA_MANAGER_HPP_INCLUDED

#include <map>
#include <variant>
#include <string>
#include <algorithm>
#include <stdexcept>
#include "Bucket.hpp"


class MetadataManager {

    std::map<std::string, std::variant<int, float, std::string>> collection;

public:

    MetadataManager() = default;

    ~MetadataManager() = default;

    inline bool has_key(const std::string& key) const { return (this->collection.find(key) != this->collection.end()); }

    template <typename T>
    bool get_key(const std::string& key, T& val);

    template <typename T>
    bool set_key(const std::string& key, const T& val, bool override=true);

    // Fills the dimensions and the calibration.
    void get_dimensions(Bucket& b) const;
    size_t get_n_channels() const;
};


/* ------------------------------------------------------------------------ */


template <typename T>
bool MetadataManager::get_key(const std::string& key, T& val) {
    auto found = this->collection.find(key);
    if (found == this->collection.end()) { return false; }

    try {
        val = std::get<T>(found->second);
        return true;
    } catch (std::exception& e) {
        return false;
    }

    return false;
}


template <typename T>
bool MetadataManager::set_key(const std::string& key, const T& val, bool override) {
    auto found = this->collection.find(key);
    if (found != this->collection.end()) {
        if (!override) { return false; } // The key exists and we don't want to override it.
        try { // The key exists, we want to override it, but we test the type before trying to override.
            std::get<T>(found->second);
            found->second = val;
            return true;
        } catch (std::exception& e) { // We tryied to override the value but with a different type.
            return false;
        }
    } 
    
    this->collection[key] = val; // The value doesn't exist and we create the key.
    return true;
}

#endif //META_DATA_MANAGER_HPP_INCLUDED