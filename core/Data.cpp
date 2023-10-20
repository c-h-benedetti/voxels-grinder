#include "Data.hpp"
#include "Task.hpp"
#include <cstdint>

int Data::run(Task& v) {
    return v.run(this);
}

void Data::set_proxy(std::unique_ptr<DataProxy> p) {
    this->proxy = std::move(p);
}
