#ifndef MEDIA_MANAGER_HPP_INCLUDED
#define MEDIA_MANAGER_HPP_INCLUDED

#include <map>
#include <filesystem>
#include "Media.hpp"

class MediaManager {

    std::map<int, Media> medias;
    int active_media=-1;

public:

    Media& get_active_media();
    inline int get_active_id() const { return active_media; }
    bool open_media(const std::filesystem::path& path);
};

#endif //MEDIA_MANAGER_HPP_INCLUDED