#ifndef DATA_RECEIVER_HPP_INCLUDED
#define DATA_RECEIVER_HPP_INCLUDED

#include <map>
#include <string>
#include <tuple>
#include <vector>
#include <variant>
#include <memory>
#include "ID.hpp"

namespace rcv {

    // All types handled by the receiver. Collections of these types are also handled.
    using Float  = double;
    using Int    = long long int;
    using Uint   = size_t;
    using String = std::string;
    using Char   = char;
    using Bool   = bool;

    template <typename T>
    using Vector = std::vector<T>;

    #define SUPPORTED_TYPES Float, Uint, String, Char, Bool, Int

    template <typename... Types>
    using typeEntry = std::variant<Types..., std::vector<Types>...>;

    template <typename... Types>
    using typeMapping = std::map<ID, typeEntry<Types...>>;

    using receivedEntry = typeEntry<SUPPORTED_TYPES>;

    using receivedMap = typeMapping<SUPPORTED_TYPES>;

};

#endif //DATA_RECEIVER_HPP_INCLUDED