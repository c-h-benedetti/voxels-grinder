#ifndef MEDIA_MANAGER_HPP_INCLUDED
#define MEDIA_MANAGER_HPP_INCLUDED

#include <map>
#include <mutex>
#include "Media.hpp"

class MediaManager {

    static std::unique_ptr<MediaManager> mediaManagerInstance;
    
    std::map<ID, media_ptr> medias;
    mutable std::mutex list_mutex;

private:

    MediaManager()  = default;
    ~MediaManager();

    MediaManager(const MediaManager&)            = delete;
    MediaManager(MediaManager&&)                 = delete;
    MediaManager& operator=(const MediaManager&) = delete;
    MediaManager& operator=(MediaManager&&)      = delete;

    friend struct std::default_delete<MediaManager>;

public:

    Media& createMedia(const std::filesystem::path& title, size_t width, size_t height, bit_depth depth, size_t nChannels=1, size_t nSlices=1, size_t nFrames=1);
    Media& createMedia(const std::filesystem::path& title, const Dimensions& dims);
    Media* getMedia(const ID& id);
    Media* getMedia(const std::filesystem::path& title);

    int close(const ID& id);
    inline size_t nOpenedMedias() const { return this->medias.size(); }

    void destroyRessources();

    static inline MediaManager& getInstance() { return *MediaManager::mediaManagerInstance; }
};

#endif // MEDIA_MANAGER_HPP_INCLUDED