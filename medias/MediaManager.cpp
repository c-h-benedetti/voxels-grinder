#include "MediaManager.hpp"

std::unique_ptr<MediaManager> MediaManager::mediaManagerInstance(new MediaManager);


/**
 * @brief Instanciates a new Media with an associated block of data.
 *
 * Medias are managed by this class and need to be known by the instance of this class (unique to each instance of the application).
 * Hence, the only way to instanciate a Media is through this method.
 *
 * @param title Name given to this Media, corresponds to both the displayed name and the name used to save the Media to the disk.
 * @param width Width of the images' canvas.
 * @param height Heught of the images' canvas.
 * @param depth Bit depth used for this Media
 * @param nChannels Number of channels.
 * @param nSlices Number of slices.
 * @param nFrames Number of frames.
 */
Media& MediaManager::createMedia(const std::filesystem::path& title, size_t width, size_t height, bit_depth depth, size_t nChannels, size_t nSlices, size_t nFrames) {
    ID id = ID::makeID();
    media_ptr med(
        new Media(
            id,
            *this,
            title,
            width,
            height,
            depth,
            nChannels,
            nSlices,
            nFrames
        ),
        [](Media* m) {
            delete m;
        }
    );

    std::lock_guard<std::mutex> lock(this->list_mutex);
    this->medias[id] = std::move(med);

    return *(this->medias[id]);
}

Media& MediaManager::createMedia(const std::filesystem::path& title, const Dimensions& dims) {
    return this->createMedia(title, dims.width, dims.height, dims.depth, dims.nChannels, dims.nSlices, dims.nFrames);
}

Media* MediaManager::getMedia(const ID& id) {
    auto it = this->medias.find(id);
    if (it != this->medias.end()) { return it->second.get(); }
    return nullptr;
}

Media* MediaManager::getMedia(const std::filesystem::path& title) {
    for (auto& it : this->medias) {
        if (it.second->getTitle() == title) { return it.second.get(); }
    }
    return nullptr;
}

int MediaManager::close(const ID& id) {
    auto it = this->medias.find(id);
    if (it != this->medias.end()) {
        it->second->release();
        this->medias.erase(it);
        return 0;
    }
    return -1;
}

void MediaManager::destroyRessources() {
    for (auto& it : this->medias) {
        it.second->release();
    }
    this->medias.clear();
}

MediaManager::~MediaManager() {
    this->destroyRessources();
}
