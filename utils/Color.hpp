#ifndef COLOR_PROCESSING_BASE_HPP_INCLUDED
#define COLOR_PROCESSING_BASE_HPP_INCLUDED

#include <array>
#include <cinttypes>
#include <initializer_list>

class Color {

public:

    enum class color_mode {
        RGB,
        HSL,
        LAB
    };

private:

    std::array<float, 3> color;
    color_mode mode = color_mode::RGB;

    constexpr explicit Color(std::array<float, 3> l, color_mode m) : color(l), mode(m) {}

    friend struct colors;

public:

    explicit Color(float x, float y, float z, color_mode m);
    explicit Color(uint8_t x, uint8_t y, uint8_t z, color_mode m);
    explicit Color(uint16_t x, uint16_t y, uint16_t z, color_mode m);

    bool set(float x, float y, float z);
    void get(float* x, float* y, float* z);
    float getX() const;
    float getY() const;
    float getZ() const;

    void asByte(uint8_t* x, uint8_t* y, uint8_t* z);
    void asShort(uint16_t* x, uint16_t* y, uint16_t* z);
    void asFloat(float* x, float* y, float* z);
};

struct colors {

    constexpr static Color BLACK = Color({0.0f, 0.0f, 0.0f}, Color::color_mode::RGB);
    constexpr static Color WHITE = Color({1.0f, 1.0f, 1.0f}, Color::color_mode::RGB);
    constexpr static Color RED   = Color({1.0f, 0.0f, 0.0f}, Color::color_mode::RGB);
    constexpr static Color GREEN = Color({0.0f, 1.0f, 0.0f}, Color::color_mode::RGB);
    constexpr static Color BLUE  = Color({0.0f, 0.0f, 1.0f}, Color::color_mode::RGB);

    enum class pattern {
        flat,
        gradient,
        noise
    };
};

/*

Les valeurs 

*/

#endif // COLOR_PROCESSING_BASE_HPP_INCLUDED