#include "MetadataManager.hpp"

void MetadataManager::get_calibration(Calibration& c) {
    c.size_x = 1.0f;
    c.size_y = 1.0f;
    c.size_z = 1.0f;

    c.size_unit_x = Calibration::SizeUnit::pixels;
    c.size_unit_y = Calibration::SizeUnit::pixels;
    c.size_unit_z = Calibration::SizeUnit::pixels;
    
    c.time_interval = 0.0f;
    c.time_unit = Calibration::TimeUnit::seconds;
    
    this->get_key<float>(VOX_SIZE_X, c.size_x);
    this->get_key<float>(VOX_SIZE_Y, c.size_y);
    this->get_key<float>(VOX_SIZE_Z, c.size_z);
    this->get_key<float>(TIME_INTERVAL, c.time_interval);

    std::wstring sux, suy, suz, tu;
    this->get_key<std::wstring>(UNIT_X, sux);
    this->get_key<std::wstring>(UNIT_Y, suy);
    this->get_key<std::wstring>(UNIT_Z, suz);
    this->get_key<std::wstring>(TIME_UNIT, tu);

    c.size_unit_x = txt_to_size_unit.at(sux);
    c.size_unit_y = txt_to_size_unit.at(suy);
    c.size_unit_z = txt_to_size_unit.at(suz);
    c.time_unit   = txt_to_time_unit.at(tu);
}