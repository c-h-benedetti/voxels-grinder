#ifndef MEDIA_OBJECT_HPP_INCLUDED
#define MEDIA_OBJECT_HPP_INCLUDED

#include <filesystem>
#include <functional>
#include "MediaData.hpp"
#include "ID.hpp"

class MediaManager;

class Media {

    ID id;
    MediaData processor = nullptr;
    std::filesystem::path title;
    MediaManager& manager;

private:

    Media() = delete;
    Media(const ID& imgId, MediaManager& mm, const std::filesystem::path& title, size_t width, size_t height, bit_depth depth, size_t nChannels, size_t nSlices, size_t nFrames);

    ~Media();

    void release();

public:

    inline ID getID() const { return this->id; }
    inline std::filesystem::path getTitle() const { return this->title; }
    inline MediaData getData() { return this->processor; }
    inline void setData(MediaData m) { this->processor = m; }

    void close();

    friend class MediaManager;
};

using media_ptr = std::unique_ptr<Media, std::function<void(Media*)>>;

#endif // MEDIA_OBJECT_HPP_INCLUDED