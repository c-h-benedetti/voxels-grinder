#include "Color.hpp"

Color::Color(float x, float y, float z, color_mode m) {
    this->set(x, y, z);
}

Color::Color(uint8_t x, uint8_t y, uint8_t z, color_mode m) {
    float f1, f2, f3;
    f1 = static_cast<float>(x) / 255.0f;
    f2 = static_cast<float>(y) / 255.0f;
    f3 = static_cast<float>(z) / 255.0f;
    this->set(f1, f2, f3);
}

Color::Color(uint16_t x, uint16_t y, uint16_t z, color_mode m) {

}

bool Color::set(float x, float y, float z) {
    return false;
}

void Color::get(float* x, float* y, float* z) {

}

float Color::getX() const {
    return 1.0;
}

float Color::getY() const {
    return 1.0;
}

float Color::getZ() const {
    return 1.0;
}

void Color::asByte(uint8_t* x, uint8_t* y, uint8_t* z) {

}
void Color::asShort(uint16_t* x, uint16_t* y, uint16_t* z) {

}
void Color::asFloat(float* x, float* y, float* z) {

}

/*

- La représentation RGB est la plus avantageuse car stockée sur des float, elle peut rapidement être adaptée à tout.

- Les LUTs doivent avoir une influence sur les couleurs dans d'autres représentation. Par exemple, on peut vouloir un LUT
  qui base ses valeurs sur la valeur de luminosité du HSL pour sortir une valeur.

*/