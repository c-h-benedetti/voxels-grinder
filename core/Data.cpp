#include "Data.hpp"
#include "Task.hpp"
#include <cstdint>

void Data::set_proxy(std::unique_ptr<DataProxy> p) {
    this->proxy = std::move(p);
}
