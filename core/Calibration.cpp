#include <stdexcept>
#include "Calibration.hpp"

void Calibration::set_size_unit(const std::string& s) { 
	auto it = txt_to_size_unit.find(s);
	if (it == txt_to_size_unit.end()) {
		std::string ex = "Unknown key for size unit: ";
		ex += s;
		throw std::invalid_argument(ex.c_str());
	}
	this->size_unit = it->second;
}

void Calibration::set_time_unit(const std::string& s) {
	auto it = txt_to_time_unit.find(s);
	if (it == txt_to_time_unit.end()) {
		std::string ex = "Unknown key for time unit: ";
		ex += s;
		throw std::invalid_argument(ex.c_str());
	}
	this->time_unit = it->second;
}