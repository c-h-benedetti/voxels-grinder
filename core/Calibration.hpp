#ifndef CALIBRATION_PROPERTIES_HPP_INCLUDED
#define CALIBRATION_PROPERTIES_HPP_INCLUDED

#include <map>
#include <string>
#include <algorithm>
#include <locale>
#include "glm.hpp"


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


class Calibration {

public:

    enum class TimeUnit {
        Years,
        Months,
        Weeks,
        Days,
        Hours,
        Minutes,
        Seconds,
        Milliseconds,
        Microseconds,
        Frames   
    };

    enum class SizeUnit {
        Kilometers,
        Hectometers,
        Decameters,
        Meters,
        Decimeters,
        Centimeters,
        Millimeters,
        Micrometers,
        Nanometers,
        Miles,
        Yards,
        Feet,
        Inches,
        Pixels
    };


private:

    float size_x=1.0f;
    float size_y=1.0f;
    float size_z=1.0f;
    float time_interval=1.0f;

    TimeUnit time_unit=TimeUnit::Frames;
    SizeUnit size_unit=SizeUnit::Pixels;

public:

    inline float get_size_x() const { return this->size_x; }
    inline float get_size_y() const { return this->size_y; }
    inline float get_size_z() const { return this->size_z; }
    inline float get_time_interval() const { return this->time_interval; }

    inline void set_size_x(float n) { this->size_x = (n < 0.0f) ? (0.0f) : (n); }
    inline void set_size_y(float n) { this->size_y = (n < 0.0f) ? (0.0f) : (n); }
    inline void set_size_z(float n) { this->size_z = (n < 0.0f) ? (0.0f) : (n); }
    inline void set_time_interval(float n) { this->time_interval = (n < 0.0f) ? (0.0f) : (n); }

    inline void set_dimensions(glm::vec3 v) {
        this->set_size_x(v[0]);
        this->set_size_y(v[1]);
        this->set_size_z(v[2]);
    }
    
    void set_size_unit(const std::string& s);
    void set_time_unit(const std::string& s);
    inline void set_size_unit(SizeUnit s) { this->size_unit = s; }
    inline void set_time_unit(TimeUnit s) { this->time_unit = s; }
};

static const std::map<Calibration::TimeUnit, float> time_unit_to_seconds = {
    {Calibration::TimeUnit::Years, 31536000.0f},
    {Calibration::TimeUnit::Months, 2628000.0f},
    {Calibration::TimeUnit::Weeks, 604800.0f},
    {Calibration::TimeUnit::Days, 86400.0f},
    {Calibration::TimeUnit::Hours, 3600.0f},
    {Calibration::TimeUnit::Minutes, 60.0f},
    {Calibration::TimeUnit::Seconds, 1.0f},
    {Calibration::TimeUnit::Milliseconds, 0.001f},
    {Calibration::TimeUnit::Microseconds, 0.000001f}
};

static const std::map<Calibration::SizeUnit, float> size_unit_to_meter = {
    {Calibration::SizeUnit::Kilometers, 1000.0f},
    {Calibration::SizeUnit::Hectometers, 100.0f},
    {Calibration::SizeUnit::Decameters, 10.0f},
    {Calibration::SizeUnit::Meters, 1.0f},
    {Calibration::SizeUnit::Decimeters, 0.1f},
    {Calibration::SizeUnit::Centimeters, 0.01f},
    {Calibration::SizeUnit::Millimeters, 0.001f},
    {Calibration::SizeUnit::Micrometers, 0.000001f},
    {Calibration::SizeUnit::Nanometers, 0.000000001f},
    {Calibration::SizeUnit::Miles, 1609.34f},
    {Calibration::SizeUnit::Yards, 0.9144f},
    {Calibration::SizeUnit::Feet, 0.3048f},
    {Calibration::SizeUnit::Inches, 0.0254f},
    {Calibration::SizeUnit::Pixels, 1.0f} 
};

static const std::map<std::string, Calibration::SizeUnit, case_insensitive_compare> txt_to_size_unit = {
    {"kilometer", Calibration::SizeUnit::Kilometers},
    {"kilometers", Calibration::SizeUnit::Kilometers},
    {"km", Calibration::SizeUnit::Kilometers},

    {"hectometer", Calibration::SizeUnit::Hectometers},
    {"hectometers", Calibration::SizeUnit::Hectometers},
    {"hm", Calibration::SizeUnit::Hectometers},

    {"decameter", Calibration::SizeUnit::Decameters},
    {"decameters", Calibration::SizeUnit::Decameters},
    {"dam", Calibration::SizeUnit::Decameters},

    {"meter", Calibration::SizeUnit::Meters},
    {"meters", Calibration::SizeUnit::Meters},
    {"m", Calibration::SizeUnit::Meters},

    {"decimeter", Calibration::SizeUnit::Decimeters},
    {"decimeters", Calibration::SizeUnit::Decimeters},
    {"dm", Calibration::SizeUnit::Decimeters},

    {"centimeter", Calibration::SizeUnit::Centimeters},
    {"centimeters", Calibration::SizeUnit::Centimeters},
    {"cm", Calibration::SizeUnit::Centimeters},

    {"millimeter", Calibration::SizeUnit::Millimeters},
    {"millimeters", Calibration::SizeUnit::Millimeters},
    {"mm", Calibration::SizeUnit::Millimeters},

    {"micrometer", Calibration::SizeUnit::Micrometers},
    {"micrometers", Calibration::SizeUnit::Micrometers},
    {"micron", Calibration::SizeUnit::Micrometers},
    {"microns", Calibration::SizeUnit::Micrometers},
    {"um", Calibration::SizeUnit::Micrometers},
    {"µm", Calibration::SizeUnit::Micrometers},

    {"nanometer", Calibration::SizeUnit::Nanometers},
    {"nanometers", Calibration::SizeUnit::Nanometers},
    {"nm", Calibration::SizeUnit::Nanometers},

    {"yard", Calibration::SizeUnit::Yards},
    {"yards", Calibration::SizeUnit::Yards},
    {"yd", Calibration::SizeUnit::Yards},

    {"foot", Calibration::SizeUnit::Feet},
    {"feet", Calibration::SizeUnit::Feet},
    {"ft", Calibration::SizeUnit::Feet},

    {"inch", Calibration::SizeUnit::Inches},
    {"inches", Calibration::SizeUnit::Inches},
    {"in", Calibration::SizeUnit::Inches},

    {"pixel", Calibration::SizeUnit::Pixels},
    {"pixels", Calibration::SizeUnit::Pixels},
    {"px", Calibration::SizeUnit::Pixels},
    {"pxl", Calibration::SizeUnit::Pixels},
};


static const std::map<std::string, Calibration::TimeUnit, case_insensitive_compare> txt_to_time_unit = {
    {"year", Calibration::TimeUnit::Years},
    {"years", Calibration::TimeUnit::Years},
    {"yr", Calibration::TimeUnit::Years},
    {"yrs", Calibration::TimeUnit::Years},

    {"month", Calibration::TimeUnit::Months},
    {"months", Calibration::TimeUnit::Months},
    {"mo", Calibration::TimeUnit::Months},

    {"week", Calibration::TimeUnit::Weeks},
    {"weeks", Calibration::TimeUnit::Weeks},
    {"wk", Calibration::TimeUnit::Weeks},

    {"day", Calibration::TimeUnit::Days},
    {"days", Calibration::TimeUnit::Days},
    {"d", Calibration::TimeUnit::Days},

    {"hour", Calibration::TimeUnit::Hours},
    {"hours", Calibration::TimeUnit::Hours},
    {"h", Calibration::TimeUnit::Hours},

    {"minute", Calibration::TimeUnit::Minutes},
    {"minutes", Calibration::TimeUnit::Minutes},
    {"min", Calibration::TimeUnit::Minutes},

    {"second", Calibration::TimeUnit::Seconds},
    {"seconds", Calibration::TimeUnit::Seconds},
    {"s", Calibration::TimeUnit::Seconds},

    {"millisecond", Calibration::TimeUnit::Milliseconds},
    {"milliseconds", Calibration::TimeUnit::Milliseconds},
    {"ms", Calibration::TimeUnit::Milliseconds},

    {"microsecond", Calibration::TimeUnit::Microseconds},
    {"microseconds", Calibration::TimeUnit::Microseconds},
    {"us", Calibration::TimeUnit::Microseconds},
    {"µs", Calibration::TimeUnit::Microseconds}
};

static const std::map<Calibration::SizeUnit, std::string> size_unit_to_txt = {
    {Calibration::SizeUnit::Kilometers, "km"},
    {Calibration::SizeUnit::Hectometers, "hm"},
    {Calibration::SizeUnit::Decameters, "dam"},
    {Calibration::SizeUnit::Meters, "m"},
    {Calibration::SizeUnit::Decimeters, "dm"},
    {Calibration::SizeUnit::Centimeters, "cm"},
    {Calibration::SizeUnit::Millimeters, "mm"},
    {Calibration::SizeUnit::Micrometers, "µm"},
    {Calibration::SizeUnit::Nanometers, "nm"},
    {Calibration::SizeUnit::Yards, "yd"},
    {Calibration::SizeUnit::Feet, "ft"},
    {Calibration::SizeUnit::Inches, "in"},
    {Calibration::SizeUnit::Pixels, "pixels"},
};

static const std::map<Calibration::TimeUnit, std::string> time_unit_to_txt = {
    {Calibration::TimeUnit::Years, "years"},
    {Calibration::TimeUnit::Months, "months"},
    {Calibration::TimeUnit::Weeks, "weeks"},
    {Calibration::TimeUnit::Days, "days"},
    {Calibration::TimeUnit::Hours, "h"},
    {Calibration::TimeUnit::Minutes, "min"},
    {Calibration::TimeUnit::Seconds, "s"},
    {Calibration::TimeUnit::Milliseconds, "ms"},
    {Calibration::TimeUnit::Microseconds, "µs"}
};


#endif //CALIBRATION_PROPERTIES_HPP_INCLUDED

/** TODO:
 * 
 * - [ ] Une table de correspondance pour retourner vers les valeurs textuelles (pour l'affichage).
 * - [ ] Une table de facteur pour associer la puissance de 10 à chaque unité.
 *       Le facteur par lequel il faut multiplier une certaine valeur pour passer à l'unité de référence.
 * 
 */