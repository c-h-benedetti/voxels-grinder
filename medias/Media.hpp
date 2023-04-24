#ifndef MEDIA_OBJECT_HPP_INCLUDED
#define MEDIA_OBJECT_HPP_INCLUDED

#include <filesystem>
#include <functional>
#include "MediaData.hpp"
#include "ID.hpp"

using Title = std::filesystem::path;
class MediaManager;

class Media {

    ID id;
    MediaData data = nullptr;
    Title title;
    MediaManager& manager;

private:

    Media() = delete;
    Media(const ID& imgId, MediaManager& mm, const Title& title, const Dimensions& dims);
    Media(const ID& imgId, MediaManager& mm, const Title& title, size_t width, size_t height, bit_depth depth, size_t nChannels, size_t nSlices, size_t nFrames);

    ~Media();

    void release();

public:

    inline ID getID() const { return this->id; }
    inline Title getTitle() const { return this->title; }
    inline MediaData getData() { return this->data; }
    inline void setData(MediaData m) { this->data = m; }

    void close();

    friend class MediaManager;
};

using media_ptr = std::unique_ptr<Media, std::function<void(Media*)>>;

#endif // MEDIA_OBJECT_HPP_INCLUDED