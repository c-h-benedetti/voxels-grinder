#include "Media.hpp"
#include "MediaManager.hpp"

Media::Media(const ID& imgId, MediaManager& mm, const std::filesystem::path& title, size_t width, size_t height, bit_depth depth, size_t nChannels, size_t nSlices, size_t nFrames): id(imgId), manager(mm), title(title) {

}

void Media::close() {
    this->manager.close(this->id);
}

// Releases the ressources and closes the window
void Media::release() {

}

Media::~Media() {
    
}
