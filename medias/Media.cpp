#include "Media.hpp"
#include "MediaManager.hpp"
#include "DataUtils.hpp"

Media::Media(const ID& imgId, MediaManager& mm, const Title& title, const Dimensions& dims): id(imgId), manager(mm), title(title) {
    switch (dims.depth) {
        case(bit_depth::BYTE):
            this->data = DataUtils::createByteData(dims);
        break;
        case(bit_depth::SHORT):
            this->data = DataUtils::createShortData(dims);
        break;
        case(bit_depth::FLOAT):
            this->data = DataUtils::createFloatData(dims);
        break;
        case(bit_depth::RGB):
            this->data = DataUtils::createRGBData(dims);
        break;
        default:
            throw "Unknown bit depth provided to Media constructor.";
    };
}

Media::Media(const ID& imgId, MediaManager& mm, const Title& title, size_t width, size_t height, bit_depth depth, size_t nChannels, size_t nSlices, size_t nFrames): 
    Media(imgId, mm, title, Dimensions(width, height, depth, nChannels, nSlices, nFrames)) {}

void Media::close() {
    this->manager.close(this->id);
}

// Releases the ressources and closes the window
void Media::release() {

}

Media::~Media() {
    
}
