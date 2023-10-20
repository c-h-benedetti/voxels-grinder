#ifndef META_DATA_MANAGER_HPP_INCLUDED
#define META_DATA_MANAGER_HPP_INCLUDED

#include <map>
#include <variant>
#include <string>
#include <algorithm>
#include <locale>
#include <stdexcept>

struct case_insensitive_compare {
    bool operator()(const std::wstring& a, const std::wstring& b) const {
        return std::lexicographical_compare(
            a.begin(), a.end(), 
            b.begin(), b.end(),
            [](wchar_t char1, wchar_t char2) {
                return std::tolower(char1, std::locale()) < std::tolower(char2, std::locale());
            }
        );
    }
};

constexpr wchar_t VOX_SIZE_X[] = L"vsize-x";
constexpr wchar_t VOX_SIZE_Y[] = L"vsize-y";
constexpr wchar_t VOX_SIZE_Z[] = L"vsize-z";

constexpr wchar_t UNIT_X[] = L"vunit-x";
constexpr wchar_t UNIT_Y[] = L"vunit-y";
constexpr wchar_t UNIT_Z[] = L"vunit-z";

constexpr wchar_t TIME_INTERVAL[] = L"time-interval";
constexpr wchar_t TIME_UNIT[]     = L"time-unit";

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
    {L"kilometer", Calibration::SizeUnit::kilometers},
    {L"kilometers", Calibration::SizeUnit::kilometers},
    {L"km", Calibration::SizeUnit::kilometers},

    {L"m", Calibration::SizeUnit::meters},
    {L"meter", Calibration::SizeUnit::meters},
    {L"meters", Calibration::SizeUnit::meters},

    {L"cm", Calibration::SizeUnit::centimeters},
    {L"centimeter", Calibration::SizeUnit::centimeters},
    {L"centimeters", Calibration::SizeUnit::centimeters},

    {L"mm", Calibration::SizeUnit::millimeters},
    {L"millimeters", Calibration::SizeUnit::millimeters},
    {L"millimeter", Calibration::SizeUnit::millimeters},

    {L"um", Calibration::SizeUnit::micrometers},
    {L"µm", Calibration::SizeUnit::micrometers},
    {L"micron", Calibration::SizeUnit::micrometers},
    {L"microns", Calibration::SizeUnit::micrometers},
    {L"micrometers", Calibration::SizeUnit::micrometers},
    {L"micrometer", Calibration::SizeUnit::micrometers},

    {L"nanometers", Calibration::SizeUnit::nanometers},
    {L"nanometer", Calibration::SizeUnit::nanometers},
    {L"nm", Calibration::SizeUnit::nanometers},

    {L"ft", Calibration::SizeUnit::feet},
    {L"foot", Calibration::SizeUnit::feet},
    {L"feet", Calibration::SizeUnit::feet},
    
    {L"in", Calibration::SizeUnit::inches},
    {L"inch", Calibration::SizeUnit::inches},
    {L"inches", Calibration::SizeUnit::inches},

    {L"px", Calibration::SizeUnit::pixels},
    {L"pxl", Calibration::SizeUnit::pixels},
    {L"pixel", Calibration::SizeUnit::pixels},
    {L"pixels", Calibration::SizeUnit::pixels},
};

static const std::map<std::wstring, Calibration::TimeUnit, case_insensitive_compare> txt_to_time_unit = {
    {L"year", Calibration::TimeUnit::years},
    {L"years", Calibration::TimeUnit::years},
    {L"yr", Calibration::TimeUnit::years},

    {L"month", Calibration::TimeUnit::months},
    {L"months", Calibration::TimeUnit::months},
    {L"mo", Calibration::TimeUnit::months},

    {L"day", Calibration::TimeUnit::days},
    {L"days", Calibration::TimeUnit::days},
    {L"d", Calibration::TimeUnit::days},

    {L"hour", Calibration::TimeUnit::hours},
    {L"hours", Calibration::TimeUnit::hours},
    {L"h", Calibration::TimeUnit::hours},

    {L"minute", Calibration::TimeUnit::minutes},
    {L"minutes", Calibration::TimeUnit::minutes},
    {L"min", Calibration::TimeUnit::minutes},

    {L"second", Calibration::TimeUnit::seconds},
    {L"seconds", Calibration::TimeUnit::seconds},
    {L"s", Calibration::TimeUnit::seconds},

    {L"millisecond", Calibration::TimeUnit::milliseconds},
    {L"milliseconds", Calibration::TimeUnit::milliseconds},
    {L"ms", Calibration::TimeUnit::milliseconds},

    {L"microsecond", Calibration::TimeUnit::microseconds},
    {L"microseconds", Calibration::TimeUnit::microseconds},
    {L"us", Calibration::TimeUnit::microseconds},
    {L"µs", Calibration::TimeUnit::microseconds}
};


class MetadataManager {

    std::map<std::wstring, std::variant<int, float, std::wstring>> collection;

public:

    MetadataManager() = default;

    ~MetadataManager() = default;

    inline bool has_key(const std::wstring& key) const { return (this->collection.find(key) != this->collection.end()); }

    template <typename T>
    bool get_key(const std::wstring& key, T& val);

    template <typename T>
    bool set_key(const std::wstring& key, const T& val, bool override=true);

    void get_calibration(Calibration& c);
};


/* ------------------------------------------------------------------------ */


template <typename T>
bool MetadataManager::get_key(const std::wstring& key, T& val) {
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
bool MetadataManager::set_key(const std::wstring& key, const T& val, bool override) {
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