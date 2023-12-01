#ifndef CALIBRATION_PROPERTIES_HPP_INCLUDED
#define CALIBRATION_PROPERTIES_HPP_INCLUDED

#include <map>
#include <string>
#include <algorithm>
#include <locale>

struct case_insensitive_compare {
    bool operator()(const std::string& a, const std::string& b) const {
        return std::lexicographical_compare(
            a.begin(), a.end(), 
            b.begin(), b.end(),
            [](char char1, char char2) {
                return std::tolower(char1, std::locale()) < std::tolower(char2, std::locale());
            }
        );
    }
};

constexpr char VOX_SIZE_X[] = "v-size-x";
constexpr char VOX_SIZE_Y[] = "v-size-y";
constexpr char VOX_SIZE_Z[] = "v-size-z";

constexpr char UNIT_X[] = "v-unit-x";
constexpr char UNIT_Y[] = "v-unit-y";
constexpr char UNIT_Z[] = "v-unit-z";

constexpr char TIME_INTERVAL[] = "time-interval";
constexpr char TIME_UNIT[]     = "time-unit";


struct Calibration {

    enum class TimeUnit {
        years,
        months,
        days,
        hours,
        minutes,
        seconds,
        milliseconds,
        microseconds
    };

    enum class SizeUnit {
        kilometers,
        meters,
        centimeters,
        millimeters,
        micrometers,
        nanometers,
        miles,
        feet,
        inches,
        pixels
    };

    float size_x;
    float size_y;
    float size_z;
    float time_interval;

    TimeUnit time_unit;

    SizeUnit size_unit_x;
    SizeUnit size_unit_y;
    SizeUnit size_unit_z;

};

static const std::map<std::wstring, Calibration::SizeUnit, case_insensitive_compare> txt_to_size_unit = {
    {"kilometer", Calibration::SizeUnit::kilometers},
    {"kilometers", Calibration::SizeUnit::kilometers},
    {"km", Calibration::SizeUnit::kilometers},

    {"m", Calibration::SizeUnit::meters},
    {"meter", Calibration::SizeUnit::meters},
    {"meters", Calibration::SizeUnit::meters},

    {"cm", Calibration::SizeUnit::centimeters},
    {"centimeter", Calibration::SizeUnit::centimeters},
    {"centimeters", Calibration::SizeUnit::centimeters},

    {"mm", Calibration::SizeUnit::millimeters},
    {"millimeters", Calibration::SizeUnit::millimeters},
    {"millimeter", Calibration::SizeUnit::millimeters},

    {"um", Calibration::SizeUnit::micrometers},
    {"µm", Calibration::SizeUnit::micrometers},
    {"micron", Calibration::SizeUnit::micrometers},
    {"microns", Calibration::SizeUnit::micrometers},
    {"micrometers", Calibration::SizeUnit::micrometers},
    {"micrometer", Calibration::SizeUnit::micrometers},

    {"nanometers", Calibration::SizeUnit::nanometers},
    {"nanometer", Calibration::SizeUnit::nanometers},
    {"nm", Calibration::SizeUnit::nanometers},

    {"ft", Calibration::SizeUnit::feet},
    {"foot", Calibration::SizeUnit::feet},
    {"feet", Calibration::SizeUnit::feet},
    
    {"in", Calibration::SizeUnit::inches},
    {"inch", Calibration::SizeUnit::inches},
    {"inches", Calibration::SizeUnit::inches},

    {"px", Calibration::SizeUnit::pixels},
    {"pxl", Calibration::SizeUnit::pixels},
    {"pixel", Calibration::SizeUnit::pixels},
    {"pixels", Calibration::SizeUnit::pixels},
};

static const std::map<std::wstring, Calibration::TimeUnit, case_insensitive_compare> txt_to_time_unit = {
    {"year", Calibration::TimeUnit::years},
    {"years", Calibration::TimeUnit::years},
    {"yr", Calibration::TimeUnit::years},

    {"month", Calibration::TimeUnit::months},
    {"months", Calibration::TimeUnit::months},
    {"mo", Calibration::TimeUnit::months},

    {"day", Calibration::TimeUnit::days},
    {"days", Calibration::TimeUnit::days},
    {"d", Calibration::TimeUnit::days},

    {"hour", Calibration::TimeUnit::hours},
    {"hours", Calibration::TimeUnit::hours},
    {"h", Calibration::TimeUnit::hours},

    {"minute", Calibration::TimeUnit::minutes},
    {"minutes", Calibration::TimeUnit::minutes},
    {"min", Calibration::TimeUnit::minutes},

    {"second", Calibration::TimeUnit::seconds},
    {"seconds", Calibration::TimeUnit::seconds},
    {"s", Calibration::TimeUnit::seconds},

    {"millisecond", Calibration::TimeUnit::milliseconds},
    {"milliseconds", Calibration::TimeUnit::milliseconds},
    {"ms", Calibration::TimeUnit::milliseconds},

    {"microsecond", Calibration::TimeUnit::microseconds},
    {"microseconds", Calibration::TimeUnit::microseconds},
    {"us", Calibration::TimeUnit::microseconds},
    {"µs", Calibration::TimeUnit::microseconds}
};


#endif //CALIBRATION_PROPERTIES_HPP_INCLUDED