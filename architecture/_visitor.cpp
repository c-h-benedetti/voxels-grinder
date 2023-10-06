#include <iostream>
#include <vector>
#include <thread>
#include <queue>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <exception>
#include <ctime>
#include <atomic>
#include <utility>
#include "gtest/gtest.h"


struct Task {

protected:

    std::string name;
    
    Task() = delete;
    Task(const std::string& n) : name(n) {}

public:
    virtual ~Task() = default;

    virtual int runner(Image8&) {
        std::string m1 = "Operation `";
        std::string m2 = "` is not available for type Image8.";
        std::string m  = m1 + name + m2;
        throw std::runtime_error(m.c_str());
    }

    virtual int runner(Image16&) {
        std::string m1 = "Operation `";
        std::string m2 = "` is not available for type Image16.";
        std::string m  = m1 + name + m2;
        throw std::runtime_error(m.c_str());
    }

    virtual Data::data_type run() = 0;
};

template <typename T>
struct Image : public Data {
    
    size_t height;
    size_t width;
    size_t nSlices;
    size_t nChannels;
    size_t nFrames;

    Bucket loaded;

    T* data = nullptr;

    Image() = default;

    Image(int h, int w, int s, int c, int f, Data::data_type p) : Data(p), height(h), width(w), nSlices(s), nChannels(c), nFrames(f) {
        data = new T[width * height * nSlices * nChannels * nFrames];
    }

    inline size_t length() const { return height * width * nChannels * nSlices * nFrames; }

    void increment_init() {
        for (size_t i = 0 ; i < this->length() ; i++) {
            data[i] = static_cast<T>(i);
        }
    }

    virtual Image* alike() = 0;

    virtual ~Image() override {
        delete[] data;
    }
};

struct Image8 : public Image<uint8_t> {

    Image8() = default;

    Image8(int h, int w, int s, int c, int f) : Image<uint8_t>(h, w, s, c, f, Data::data_type::IMG_U8) {}

    Image8* alike() override {
        return new Image8();
    }

    int run(Task& v) override {
        return v.runner(*this);
    }
};

struct Image16 : public Image<uint16_t> {

    Image16() = default;

    Image16(int h, int w, int s, int c, int f) : Image<uint16_t>(h, w, s, c, f, Data::data_type::IMG_U16)  {}

    Image16* alike() override {
        return new Image16();
    }

    int run(Task& v) override {
        return v.runner(*this);
    }
};


struct ThresholdTask : Task {

    float t_value;
    Data* target;
    Data* result;

    ThresholdTask(float t, Data* d=nullptr) : Task("Threshold"), t_value(t), target(d) {

    }

    void set_target(Data* d) {
        target = d;
    }

    template <typename T, typename G, int K>
    static void threshold(T& c, G t, T& result, Bucket b) {
        std::string s = (K == 8) ? "Threshold (Image8)" : "Threshold (Image16)";
    }

    int runner(Image8& c) override {
        
        if (t_value > 255)
            throw std::runtime_error("Threshold value out of range");
        if (t_value < 0)
            throw std::runtime_error("Threshold value out of range");
        
        Image8* r = c.alike();
        result = r;        
        uint8_t t = static_cast<uint8_t>(t_value);

        ThreadPool::threadpool()->delegate_task(
            [&] (Bucket b) -> bool {
                threshold<Image8, uint8_t, 8>(c, t, *r, b);
                return true;
            },
            c.get_buckets()
        );

        return 1;
    }

    int runner(Image16& c) override {
        
        if (t_value > 65535)
            throw std::runtime_error("Threshold value out of range");
        if (t_value < 0)
            throw std::runtime_error("Threshold value out of range");
        
        Image16* r = c.alike();
        result = r;
        u_int16_t t = static_cast<uint16_t>(t_value);
        
        ThreadPool::threadpool()->delegate_task(
            [&] (Bucket b) -> bool {
                threshold<Image16, uint16_t, 16>(c, t, *r, b);
                return true;
            },
            c.get_buckets()
        );

        return 1;
    }

    Data::data_type run() override {
        target->run(*this);
        return target->dtype_idx;
    }
};

struct ImageCalculatorTask : Task {

    enum class Operation {
        NONE,
        ADDITION,
        SUBTRACTION,
        MULTIPLICATION,
        DIVISION,
        ABS_DIFFERENCE,
        MAXIMUM,
        MINIMUM
    };

    Operation operation = Operation::NONE;
    Data* img1;
    Data* img2;
    Data* result;

    void set_operation(Operation o) {
        operation = o;
    }

    void set_targets(Data* d1, Data* d2) {
        img1 = d1;
        img2 = d2;
    }

    ImageCalculatorTask(Data* d1=nullptr, Data* d2=nullptr) : Task("ImageCalculator"), img1(d1), img2(d2) {

    }

    template <typename T>
    static T* addition(T& c1, T& c2) {
        std::cout << "Ran ImageCalculator: ADDITION" << std::endl;
        return new T();
    }

    int runner(Image8& c1) override {
        
        Image8* c2 = dynamic_cast<Image8*>(img2);

        if (c2 == nullptr)
            throw std::runtime_error("ImageCalculatorTask: Image types do not match");
        
        switch (operation) {
            case Operation::ADDITION:
                std::cout << "Running operation: " << "addition" << std::endl;
                this->result = ImageCalculatorTask::addition<Image8>(c1, *c2);
                break;
            case Operation::SUBTRACTION:
                std::cout << "Running operation: " << "subtration" << std::endl;
                break;
            case Operation::MULTIPLICATION:
                std::cout << "Running operation: " << "multiplication" << std::endl;
                break;
            case Operation::DIVISION:
                std::cout << "Running operation: " << "division" << std::endl;
                break;
            case Operation::ABS_DIFFERENCE:
                std::cout << "Running operation: " << "difference" << std::endl;
                break;
            case Operation::MAXIMUM:
                std::cout << "Running operation: " << "maximum" << std::endl;
                break;
            case Operation::MINIMUM:
                std::cout << "Running operation: " << "minimum" << std::endl;
                break;
            case Operation::NONE:
                std::cout << "Running operation: " << "none" << std::endl;
                break;
            default:
                std::cerr << "No operation set" << std::endl;
        };

        return 1;
    }

    int runner(Image16& c1) override {
        
        Image16* c2 = dynamic_cast<Image16*>(img2);

        if (c2 == nullptr)
            throw std::runtime_error("ImageCalculatorTask: Image types do not match");
        
        switch (operation) {
            case Operation::ADDITION:
                std::cout << "Running operation: " << "addition" << std::endl;
                this->result = ImageCalculatorTask::addition<Image16>(c1, *c2);
                break;
            case Operation::SUBTRACTION:
                std::cout << "Running operation: " << "subtration" << std::endl;
                break;
            case Operation::MULTIPLICATION:
                std::cout << "Running operation: " << "multiplication" << std::endl;
                break;
            case Operation::DIVISION:
                std::cout << "Running operation: " << "division" << std::endl;
                break;
            case Operation::ABS_DIFFERENCE:
                std::cout << "Running operation: " << "difference" << std::endl;
                break;
            case Operation::MAXIMUM:
                std::cout << "Running operation: " << "maximum" << std::endl;
                break;
            case Operation::MINIMUM:
                std::cout << "Running operation: " << "minimum" << std::endl;
                break;
            case Operation::NONE:
                std::cout << "Running operation: " << "none" << std::endl;
                break;
            default:
                std::cerr << "No operation set" << std::endl;
        };

        return 1;
    }

    Data::data_type run() override {
        img1->run(*this);
        return img1->dtype_idx;
    }
};

struct MaximumTask : Task {

    float max_value;
    Data* target;

    MaximumTask(Data* d=nullptr) : Task("Maximum"), target(d) {};

    void set_target(Data* d) {
        target = d;
    }

    static uint8_t maximum(Image8& c) {
        std::cout << "Search for maximum in an Image8" << std::endl;
        return 125;
    }

    int runner(Image8& c) override {
        this->max_value = maximum(c);
        return 1;
    }

    Data::data_type run() override {
        target->run(*this);
        return target->dtype_idx;
    }
};

/** Étapes d'une exécution de task:
 * 
 * 1. On instancie la task.
 * 2. Les paramètres sont passés à la task soit à travers le constructeur ou des setters.
 * 3. Le run est appelé et utilise l'image principale pour déterminer le type du résultat.
 * 4. Le run de la task appelle le run de l'image pour avoir le bon type dynamique au-delà du type statique qui sera souvent Data*.
 * 5. Le run de l'image appelle le runner de la task avec le bon type.
 * 6. À partir de là, la task connait le type dynamique des données, et on est dans le corps du bon runner.
 * 7. La task doit maintenant appeler la fonction de preprocess.
 * 8. Une fois qu'elle est terminée, c'est maintenant que l'appel au thread pool doit être fait. La task doit avoir des conteneurs pour le résultat, utilisés par le thread pool.
 * 9. Quand le thread pool a terminé, la task doit appeler la fonction de postprocess pour assembler le résultat.
 * 
 */

// - [ ] Créer une petite image.
// - [ ] Créer une image assez grande pour qu'elle passe le threshold.
// - [ ] Charger une petite image.
// - [ ] Charger une image assez grande pour dépasser le threshold.

TEST(TaskUse, DelegationAvailable) {
    RecordProperty("description", "Is the correct implementation called for every type of image?");
    
	Image8  c1(512, 512, 5, 3, 9);
    Image16 c2(512, 512, 5, 3, 9);
    Image16 c3(512, 512, 5, 3, 9);

    c1.increment_init();
    c2.increment_init();
    c3.increment_init();

    ThresholdTask t1(31.0f, &c1);
    ThresholdTask t2(31.0f, &c2);
    ThresholdTask t3(31.0f, &c3);

    Data::data_type r1 = t1.run();
    Data::data_type r2 = t2.run();
    Data::data_type r3 = t3.run();

    EXPECT_EQ(r1, Data::data_type::IMG_U8);
    EXPECT_EQ(r2, Data::data_type::IMG_U16);
    EXPECT_EQ(r3, Data::data_type::IMG_U16);

    Data* p1 = &c1;
    Data* p2 = &c2;
    Data* p3 = &c3;

    t1.set_target(p1);
    t2.set_target(p2);
    t3.set_target(p3);

    r1 = t1.run();
    r2 = t2.run();
    r3 = t3.run();

    EXPECT_EQ(r1, Data::data_type::IMG_U8);
    EXPECT_EQ(r2, Data::data_type::IMG_U16);
    EXPECT_EQ(r3, Data::data_type::IMG_U16);
}


TEST(TaskUse, DelegationUnavailable) {
    Image8  c1(512, 512, 5, 3, 9);
    Image16 c2(512, 512, 5, 3, 9);
    Image16 c3(512, 512, 5, 3, 9);

    c1.increment_init();
    c2.increment_init();
    c3.increment_init();

    MaximumTask m1(&c1);
    MaximumTask m2(&c2);
    MaximumTask m3(&c3);

    Data::data_type r1 = Data::data_type::UNDEFINED;
    Data::data_type r2 = Data::data_type::UNDEFINED;
    Data::data_type r3 = Data::data_type::UNDEFINED;

    EXPECT_NO_THROW({
        r1 = m1.run();
    });

    EXPECT_ANY_THROW({
        r2 = m2.run();
    });

    EXPECT_ANY_THROW({
        r3 = m3.run();
    });
    
    EXPECT_EQ(r1, Data::data_type::IMG_U8);
    EXPECT_EQ(r2, Data::data_type::UNDEFINED);
    EXPECT_EQ(r3, Data::data_type::UNDEFINED);
}


int main() {
    testing::GTEST_FLAG(output) = "xml:./architecture_tests.xml";

    testing::InitGoogleTest();

    if (RUN_ALL_TESTS()){
        std::cerr << "At least a test failed" << std::endl;
    }

	return 0;
}
