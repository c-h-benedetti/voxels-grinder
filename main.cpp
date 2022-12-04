#include <iostream>
#include <map>
#include "Image.hpp"

// - Ecrire les accesseurs sur l'image

int main(int argc, char* argv[], char* env[]) {

    Image<uint16_t> img1;
    Image<double> img2(1024, 1024, 31, 2, 25, 0.0);
    // Image<int> img3("my/path/with/name.png");
    // Image<uint8_t> img4 = img3;

    // img1.write("my/path/with/name.png");
    // img2.write(ImageFormat::TIFF);

    return 0;
}
