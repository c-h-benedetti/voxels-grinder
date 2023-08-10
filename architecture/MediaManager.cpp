#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include <filesystem>
#include <utility>

namespace fs = std::filesystem;

class Data {
public:
    virtual ~Data() {}
};

class Mesh : public Data {
public:
    virtual ~Mesh() {}
};

enum data_type {
    none,
    ui8,
    ui16,
    f32,
    rgb,
    labeling,
    mask
};

// We will stick to one channel and one frame at a time.
struct BoundingBox {
    size_t ulb[3] = {0}; // upper left back
    size_t lrf[3] = {0}; // lower right front
};

class Image : public Data {
protected:

    const data_type dtype;
    size_t height;
    size_t width;
    size_t nSlices;
    size_t nChannels;
    size_t nFrames;

protected:
    
    Image(): dtype(data_type::none) {}
    Image(data_type dt): dtype(dt) {}    
    Image(data_type dt, size_t h, size_t w, size_t s, size_t c, size_t f): dtype(dt), height(h), width(w), nSlices(s), nChannels(c), nFrames(f) {}

public:
    
    virtual ~Image() {}
};

class Image8 : public Image {
public:
    Image8(): Image(data_type::ui8) {}
    Image8(size_t h, size_t w, size_t s, size_t c, size_t f): Image(data_type::ui8, h, w, s, c, f) {}
};

class Mask : public Image8 {
public:
    Mask(): Image8() {}
    Mask(size_t h, size_t w, size_t s, size_t c, size_t f): Image8(h, w, s, c, f) {}
};

class Image16 : public Image {
public:
    Image16(): Image(data_type::ui16) {}
};

class Image32 : public Image {
public:
    Image32(): Image(data_type::f32) {}
};

class ImageRGB : public Image {
public:
    ImageRGB(): Image(data_type::rgb) {}
};

class Labeling : public Image {
public:
    Labeling(): Image(data_type::labeling) {}
};

class Media {
    std::u32string str32 = U"é 猫 🐱";
    std::unique_ptr<Data> data;
public:
    bool rename(const std::u32string& name) { return false; }
    friend class MediaManager;
};


class io_worker {
public:
    virtual ~io_worker() {}
    virtual Data* get_data(const fs::path& p) = 0;
};

class tiff_io : public io_worker {};
class png_io : public io_worker {};
class obj_io : public io_worker {};

class DataIO {

    std::map<std::u32string, io_worker*> extension_to_worker;
    static std::unique_ptr<DataIO> instance;

public:

    Data* open(const fs::path& p) { return nullptr; }

    bool saveAs(Data* d, const fs::path& p) { return true; }

    static inline DataIO* getInstance() {
        if (DataIO::instance == nullptr) {
            DataIO::instance = std::unique_ptr<DataIO>(new DataIO);
        }
        return DataIO::instance.get();
    }
};

std::unique_ptr<DataIO> DataIO::instance = nullptr;

class MediaManager {
    std::vector<Media*> media_instances;
    std::map<std::u32string, size_t> media_indices;

public:

    Media* getMedia(const std::u32string& name) { return nullptr; }
    void registerMedia(const std::u32string& name, Data* data) {}
};


struct AutoThreshold {
    enum algorithm {
        otsu,
        li,
        triangle,
        yen,
        intermode,
        median
    };

    template <typename I, typename T>
    static T run(I img, algorithm a=algorithm::intermode); // { return 0; }
};

template <>
uint8_t AutoThreshold::run<Image8*, uint8_t>(Image8* img, algorithm a)     { return 1; }

template <>
uint16_t AutoThreshold::run<Image16*, uint16_t>(Image16* img, algorithm a) { return 2; }

template <>
float AutoThreshold::run<Image32*, float>(Image32* img, algorithm a)       { return 3.1415629; }

template <>
uint8_t AutoThreshold::run<ImageRGB*, uint8_t>(ImageRGB* img, algorithm a) { return 42; }

//********************************************************************

struct Threshold {
    template <typename I, typename T>
    static Image8* run(I img, T val); // { return nullptr ; }
};

template <>
Image8* Threshold::run<Image8*, uint8_t>(Image8* img, uint8_t val)     { return nullptr; }

template <>
Image8* Threshold::run<Image16*, uint16_t>(Image16* img, uint16_t val) { return nullptr; }

template <>
Image8* Threshold::run<Image32*, float>(Image32* img, float val)       { return nullptr; }

template <>
Image8* Threshold::run<ImageRGB*, uint8_t>(ImageRGB* img, uint8_t val) { return nullptr; }


struct SplitChannels {
    template <typename I>
    static std::vector<I> run(I img); // { return nullptr ; }
};

template <>
std::vector<Image8*> SplitChannels::run<Image8*>(Image8* img)      { return {nullptr}; }

template <>
std::vector<Image16*> SplitChannels::run<Image16*>(Image16* img)  { return {nullptr, nullptr}; }

template <>
std::vector<Image32*> SplitChannels::run<Image32*>(Image32* img)      { return {nullptr, nullptr, nullptr, nullptr}; }

template <>
std::vector<ImageRGB*> SplitChannels::run<ImageRGB*>(ImageRGB* img) { return {nullptr, nullptr, nullptr}; }

struct RandomForestWeights {};

namespace PixelClassifier {

    struct classifier_data { // needs settings (such as filters, sizes for each filter, ...)
        std::vector<std::u32string> classes; // Position corresponds to label.
        RandomForestWeights* rfw = nullptr; // Weights of random forest.
    };

    struct Training {
        template <typename I>
        void run(I img, Labeling* labeling, classifier_data* cd);
    };
    
    struct Predict {
        template <typename I>
        Labeling run(I img, classifier_data* cd);
    };
};


namespace MaskMorphology {
    Image8* dilate(Image8* img); // A changer en 'Mask'
    Image8* erode(Image8* img);
    Image8* open(Image8* img);
    Image8* close(Image8* img);
    Image8* chamferDistance(Image8* img);
};


namespace LabelsMorphology {

};


class Worker {};

class ThreadPool {

    std::vector<Worker*> workers;
    std::vector<bool>    state;

public:

};


int main() {
    // ===== Experiment: Seg'n'track (Thomas Sabaté) =====

    // 1. Open and load image from file + split channels.
    Image16* img = dynamic_cast<Image16*>(DataIO::getInstance()->open("/some/path/to/image.tif"));
    std::vector<Image16*> channels = SplitChannels::run(img);
    delete img;
    Image16* red   = channels[0];
    Image16* green = channels[1];

    // 2. Separate cells by thresholding at a certain value.
    uint16_t t  = AutoThreshold::run<Image16*, uint16_t>(red, AutoThreshold::algorithm::otsu);
    Image8* res = Threshold::run<Image16*, uint16_t>(red, t);

    // N. Save result as a new image.
    DataIO::getInstance()->saveAs(res, "/some/path/to/mask.tif");

    return 0;
}
