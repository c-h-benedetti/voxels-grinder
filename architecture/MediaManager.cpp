#include <iostream>
#include <map>
#include <vector>
#include <memory>
#include <filesystem>
#include <map>
#include <utility>
#include <set>
#include <limits>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <functional>

namespace fs = std::filesystem;

struct vec2 { float data[2]; };
struct vec3 { float data[3]; };
struct vec4 { float data[4]; };

// Doit-on avoir des buckets pour les images et des buckets pour les meshes ?
// Pour les images, les buckets seraient des bounding boxes.
// Pour les meshes ça serait des ranges d'indices de vertices.
struct Bucket {
    vec3 lfb; // lower left backward
    vec3 urf; // upper right front
    size_t c; // channel
    size_t f; // frame
};

namespace Calibration {
    enum class length_unit {
        km,
        hm,
        dam,
        m,
        dm,
        cm,
        mm,
        um,
        nm,
        pm,
        fm
    };

    enum class time_unit {
        y,
        d,
        h,
        m,
        s,
        ms,
        us
    };

    class Calibration {
        // 1 voxel corresponds to...
        std::pair<float, length_unit> x;
        std::pair<float, length_unit> y;
        std::pair<float, length_unit> z;
        std::pair<float, time_unit>   t;

        vec4 asVector() const {
            return {x.first, y.first, z.first, t.first};
        }
    };

    vec4 pixelsToCalibration(const vec4& v1) {
        // Values in `v` are stored in number of pixels.
        return v1;
    }

};

enum DataType : uint32_t {
    none = 0,
    u8 = 0 << 1,
    u16 = 0 << 2,
    sfloat = 0 << 3,
    rgb = 0 << 4,
    labels = 0 << 5,
    mask = 0 << 6
};

constexpr uint32_t image_flag = DataType::u8 | DataType::u16 | DataType::sfloat | DataType::rgb | DataType::labels | DataType::mask;

class Data {
    std::queue<Bucket*> buckets;
public:
    virtual ~Data() {}
    virtual void close() {}
    inline std::queue<Bucket*> get_buckets() const { return this->buckets; }
};

// L'attribut qui contient les buckets devrait être déclaré dans Image et Mesh plutôt que Data.
// On pourrait ainsi donner un type de bucket spécialisé.

//##################################################

struct segment { size_t data[2]; };
struct triangle { size_t data[3]; };

// Sachant que les points d'une sélection sont bound à un channel et une frame, on voudra un vec5 pour eux.
// On pourrait aussi faire une collection de vec2 à côté pour pouvoir faire les transferts GPU plus rapidement.
class Vertices : public Data {
protected:
    std::vector<vec3> vertices;
};

class Polyline : public Vertices {
protected:
    std::vector<segment> segments;
};

class Polygon : public Polyline {
    // The only difference is that the last segment and the first are considered closed.
};

class Mesh : public Vertices {
    std::vector<triangle> triangles;
};

//##################################################

struct Dimensions {
    size_t height;
    size_t width;
    size_t nSlices;
    size_t nChannels;
    size_t nFrames;
};


template <typename T>
class Image : public Data {

    Dimensions dimensions;

public:

    inline Dimensions getDimensions() const { return dimensions; }
    static DataType getType() { return none; }

    virtual ~Image() {}
};

class ImageU8 : public Image<uint8_t> {
public:
    static DataType getType() { return DataType::u8; }
};

class ImageU16 : public Image<uint16_t> {
public:
    static DataType getType() { return DataType::u16; }
};

class ImageFloat : public Image<float> {
public:
    static DataType getType() { return DataType::sfloat; }
};

class ImageRGB : public Image<uint8_t> {
public:
    static DataType getType() { return DataType::u8; }
};

class ImageLabeling : public Image<uint64_t> {
public:
    static DataType getType() { return DataType::labels; }
};

class ImageMask : public Image<bool> {
public:
    static DataType getType() { return DataType::mask; }
};

//##################################################

ImageU8* duplicate(ImageU8* img) { return nullptr; }
ImageU16* duplicate(ImageU16* img) { return nullptr; }
ImageFloat* duplicate(ImageFloat* img) { return nullptr; }
ImageRGB* duplicate(ImageRGB* img) { return nullptr; }
ImageLabeling* duplicate(ImageLabeling* img) { return nullptr; }
ImageMask* duplicate(ImageMask* img) { return nullptr; }

//##################################################

// Undefined for masks and rgb.
void threshold(ImageU8* img, uint8_t lower=0, uint8_t upper=std::numeric_limits<uint8_t>::max()) {}
void threshold(ImageU16* img, uint16_t lower=0, int16_t upper=std::numeric_limits<uint16_t>::max()) {}
void threshold(ImageFloat* img, float lower=0.0f, float upper=1.0f) {}
void threshold(ImageLabeling* img, uint64_t lower=0, int64_t upper=std::numeric_limits<uint64_t>::max()) {}

//##################################################

enum threshold_type {
    INTERMODE,
    OTSU,
    YEN,
    MEDIAN,
    TRIANGLE
};

uint8_t auto_threshold(const ImageU8* img, threshold_type tt) { return 0; }
uint16_t auto_threshold(const ImageU16* img, threshold_type tt) { return 0; }
float auto_threshold(const ImageFloat* img, threshold_type tt) { return 0.0f; }
uint64_t auto_threshold(const ImageLabeling* img, threshold_type tt) { return 0; }

//##################################################

std::vector<ImageU8*> split_channels(const ImageU8* img) { return {nullptr}; }
std::vector<ImageU16*> split_channels(const ImageU16* img) { return {nullptr, nullptr}; }
std::vector<ImageFloat*> split_channels(const ImageFloat* img) { return {nullptr}; }
std::vector<ImageRGB*> split_channels(const ImageRGB* img) { return {nullptr, nullptr, nullptr}; }
std::vector<ImageLabeling*> split_channels(const ImageLabeling* img) { return {nullptr, nullptr}; }
std::vector<ImageMask*> split_channels(const ImageMask* img) { return {nullptr}; }

//##################################################

ImageU8* enhance_contrast(ImageU8* img, float skip) { return nullptr; }
ImageU16* enhance_contrast(ImageU16* img, float skip) { return nullptr; }
ImageFloat* enhance_contrast(ImageFloat* img, float skip) { return nullptr; }
ImageRGB* enhance_contrast(ImageRGB* img, float skip) { return nullptr; }

//##################################################

template <typename I>
class RandomForestSegmentation {
    // Si on veut update les labels à chaque essaie, il va falloir changer la façon dont-on fonctionne.
    // On veut garder l'aspect "programmation fonctionnelle", mais on voudrait que certaines fonctions puissent modifier le canvas qu'elles reçoivent.
    // Les fonctions devraient peut-être modifier le canvas quand c'est possible, et ça sera aux actions de dupliquer ou non l'image.
    // Avec les pointeurs, il est facile de garder le résultat.
    std::vector<const I*> intensity_images;
    std::vector<ImageLabeling*> ground_truths;

public:

    bool load_weights(const fs::path& p) { return true; }
    void reset_training_set() {}
    bool add_to_training_set(ImageLabeling* ground_truth, I* image) { return true; }
    void train() {}
    ImageLabeling* predict(I* image) const { return nullptr; }
};

//##################################################

class type_io {};

class DataIO {
    
    std::map<const std::u32string, std::unique_ptr<type_io>> type_managers;
    static std::unique_ptr<DataIO> data_io;

public:

    template <typename T>
    T* openFromDisk(const fs::path& p) { return nullptr; /*return dynamic_cast<T*>(nullptr);*/ }

    template <typename T>
    T* openFromNetwork(const std::u32string& url) { return nullptr; /*return dynamic_cast<T*>(nullptr);*/ }
    
    bool saveAs(Data*, const fs::path& p) { return true; }

    static DataIO& instance() {
        if (data_io == nullptr) { data_io = std::unique_ptr<DataIO>(new DataIO); }
        return *(data_io.get());
    }
};

std::unique_ptr<DataIO> DataIO::data_io = std::unique_ptr<DataIO>(nullptr);

//##################################################

namespace MasksMorphology {
    enum class shape {
        circle_sphere,
        rectangle_box
    };

    struct StructuringElement {
        StructuringElement(size_t size_x, size_t size_y, size_t size_z, shape s) {}
    };

    void erode(ImageMask* img) {}
    void dilate(ImageMask* img) {}
    void open(ImageMask* img) {}
    void close(ImageMask* img) {}
    void chamfer_distance(ImageMask* img) {}
};

namespace LabelsMorphology {
    void select_labels(ImageLabeling* img, const std::set<uint64_t>& labels) {
        /// Modifies the input image by setting values not in `labels` to 0.
    }

    ImageMask* mask_from_labels(ImageLabeling* img, const std::set<uint64_t>& labels) { 
        /// Returns a mask in which values that were in `labels` were set to true. false otherwise.
        return nullptr;
    }

    ImageLabeling* connected_components_labeling(ImageMask* img, uint8_t connectivity) { return nullptr; }
};

//##################################################

class ThreadPool {

    std::vector<std::thread> workers;
    std::queue<Bucket*> tasks;
    std::mutex queueMutex;
    std::condition_variable condition;
    bool stop = false;
    std::function<bool(Bucket*)> function;

    static std::unique_ptr<ThreadPool> tp;

public:

    ThreadPool(size_t threads) {
        for(size_t i = 0; i < threads; ++i) {
            workers.emplace_back([this] {
                while(true) {
                    Bucket* task;
                    {
                        std::unique_lock<std::mutex> lock(this->queueMutex);
                        this->condition.wait(lock, [this] { return this->stop || !this->tasks.empty(); });
                        if(this->stop && this->tasks.empty()) return;
                        task = tasks.front();
                        tasks.pop();
                    }
                    function(task);
                }
            });
        }
    }

    void set_task(std::function<bool(Bucket*)> f) {
        function = f;
    }

    void transfer_buckets(const Data* d) {
        this->tasks = d->get_buckets();
    }

    void enqueueTask(Bucket* task) {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            if(stop) throw std::runtime_error("enqueue on stopped ThreadPool");
            tasks.push(task);
        }
        condition.notify_one();
    }

    ~ThreadPool() {
        {
            std::unique_lock<std::mutex> lock(queueMutex);
            stop = true;
        }
        condition.notify_all();
        for(std::thread &worker: workers) {
            worker.join();
        }
    }

    static size_t nThreads() {
        // Doit calculer le nombre de threads optimal en fonction du CPU dispo.
        return 16;
    }

    static ThreadPool* instance() {
        if (tp == nullptr) {
            tp = std::unique_ptr<ThreadPool>(new ThreadPool(nThreads()));
        }
        return tp.get();
    }
};

std::unique_ptr<ThreadPool> ThreadPool::tp = nullptr;

//##################################################

// Peut-être que la fonction threshold devrait être un template et que les spécialisations devraient se faire à travers les méthodes de la classe.

// Cette classe doit prendre tous les buckets de l'image et appliquer la fonction dessus.
// Les fonctions vont devoir prendre un bucket en paramètre pour savoir où s'arrêter dans leur calcul.
// Cette classe doit attendre que l'opération soit terminée et que le threadpool soit libre avant de terminer.

// 1. La classe doit commencer par mettre en file d'attente tous les buckets de l'image (dupliquer la collection ?)
// 2. Chaque thread doit appeler la fonction avec le bon bucket. S'il y a un canvas d'arrivée, il est supposé être commun à tous.
// 3. S'il y a une phase de fusion, c'est que tout le monde a produit qqchose. Comment merge ? Comment récupérer tous les résultats ?
// 4. Chaque thread doit savoir où écrire son résultat quand il commence son exécution. Il ne peut rien return.

// Pour les images, faire des types qu'on peut facilement transformer en flag.
// Au moins, la fonction isImage(Data*) pourra simplement faire un bitwise and.
// Ça serait quand même bien si on pouvait éviter de faire des checks de type dans le code.

// Avant de continuer, il faut finir le cours d'embeding de Python pour faire une implémentation compatible.
class Threshold {

    ThreadPool* tp = ThreadPool::instance();

public:

    Threshold(ImageU8* d) {
        tp->transfer_buckets(d);
        tp->set_task([&](Bucket* b) -> bool {
            threshold(d);
            return true;
        });
    }

    ~Threshold() {

    }
};

//##################################################

void thomas_sabate_seg_n_track() {
    ImageU16* imIn = DataIO::instance().openFromDisk<ImageU16>("/some/path/to/image.tif");
    std::vector<ImageU16*> channels = split_channels(imIn);
    ImageU16* splitGFP = channels[0];
    ImageU16* rad21    = channels[1];
    imIn->close();

    RandomForestSegmentation<ImageU16> rfs;
    if (!rfs.load_weights("/some/path/to/weights.h5d")) { return; }
    
    ImageLabeling* raw_segmentation = rfs.predict(splitGFP);
    splitGFP->close();
    ImageMask* nuclei_mask = LabelsMorphology::mask_from_labels(raw_segmentation, {1});
}

int main(int argc, char* argv[], char* env[]) {
    std::cout << "Version 2" << std::endl;
    return 0;
}