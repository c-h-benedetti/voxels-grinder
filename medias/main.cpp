#include "MediaData.hpp"
#include "MediaManager.hpp"
#include "DataUtils.hpp"
#include "bit_depth.hpp"
#include "gtest/gtest.h"
#include <cstdint>


namespace MediaManagerTests {
    class MediaManagerTest : public ::testing::Test {
    protected:
        static void SetUpTestCase() {
            // Initialization of global resources for the test case, if needed.
        }

        static void TearDownTestCase() {
            // Clean up of global resources for the test case, if needed.
        }

        void SetUp() override {
            // Reset the static attribute before each test
        }

        void TearDown() override {
            MediaManager& mm = MediaManager::getInstance();
            mm.destroyRessources();
        }
    };


    TEST_F(MediaManagerTest, RealSingleton) {
        RecordProperty("description", "Verifies that the MediaManager respects the singleton principles.");

        MediaManager& m1 = MediaManager::getInstance();
        MediaManager& m2 = MediaManager::getInstance();

        EXPECT_EQ(&m1, &m2);
    }

    TEST_F(MediaManagerTest, DeleteFromMedia) {
        RecordProperty("description", "Closing Media from the instance itself.");

        MediaManager& mm = MediaManager::getInstance();

        std::filesystem::path title1 = "title1";
        size_t width1                = 240;
        size_t height1               = 144;
        bit_depth depth1             = bit_depth::BYTE;
        size_t nChannels1            = 2;
        size_t nSlices1              = 3;
        size_t nFrames1              = 4;

        EXPECT_EQ(mm.nOpenedMedias(), 0);
        Media& m1 = mm.createMedia(title1, width1, height1, depth1, nChannels1, nSlices1, nFrames1);
        EXPECT_EQ(mm.nOpenedMedias(), 1);
        m1.close();
        EXPECT_EQ(mm.nOpenedMedias(), 0);  
    }

    /// [Ca c'est l'ID utilisé]
    TEST_F(MediaManagerTest, createMedia) {
        RecordProperty("description", "Is a MediaManager able to instanciate and store instances of Media?");

        MediaManager& mm = MediaManager::getInstance();

        std::filesystem::path title1 = "title1";
        size_t width1                = 240;
        size_t height1               = 144;
        bit_depth depth1             = bit_depth::BYTE;
        size_t nChannels1            = 2;
        size_t nSlices1              = 3;
        size_t nFrames1              = 4;

        std::filesystem::path title2 = "title2";
        size_t width2                = 144;
        size_t height2               = 240;
        bit_depth depth2             = bit_depth::SHORT;
        size_t nChannels2            = 3;
        size_t nSlices2              = 2;
        size_t nFrames2              = 1;

        ID id = ID::makeID();

        Media& m1 = mm.createMedia(title1, width1, height1, depth1, nChannels1, nSlices1, nFrames1);
        Media& m2 = mm.createMedia(title2, width2, height2, depth2, nChannels2, nSlices2, nFrames2);

        Media* mp1 = mm.getMedia(m1.getID());
        Media* mp2 = mm.getMedia(m2.getTitle());
        Media* mp3 = mm.getMedia(id);
        Media* mp4 = mm.getMedia("some title");

        ASSERT_NE(mp1, nullptr);
        ASSERT_NE(mp2, nullptr);
        EXPECT_EQ(mp3, nullptr);
        EXPECT_EQ(mp4, nullptr);

        EXPECT_EQ(m1.getID(), mp1->getID());
        EXPECT_EQ(m2.getID(), mp2->getID());

        EXPECT_EQ(m1.getTitle(), title1);
    }
    /// [Ca c'est l'ID utilisé]


    TEST_F(MediaManagerTest, closeFromManager) {
        RecordProperty("description", "Can we close a Media from the Manager?");

        MediaManager& mm = MediaManager::getInstance();

        std::filesystem::path title1 = "title1";
        size_t width1                = 240;
        size_t height1               = 144;
        bit_depth depth1             = bit_depth::BYTE;
        size_t nChannels1            = 2;
        size_t nSlices1              = 3;
        size_t nFrames1              = 4;

        EXPECT_EQ(mm.nOpenedMedias(), 0);
        Media& m1 = mm.createMedia(title1, width1, height1, depth1, nChannels1, nSlices1, nFrames1);
        EXPECT_EQ(mm.nOpenedMedias(), 1);
        mm.close(m1.getID());
        EXPECT_EQ(mm.nOpenedMedias(), 0);
    }

    TEST_F(MediaManagerTest, createByteMedia) {
        RecordProperty("description", "Ability of a MediaManager to create a valid Media containing bytes.");

        MediaManager& mm = MediaManager::getInstance();

        std::filesystem::path title = "ByteTitle";
        Dimensions dims(240, 140, bit_depth::BYTE, 3, 4, 5);

        Media& m1 = mm.createMedia(title, dims);

        Dimensions d2 = m1.getData()->getDimensions();
        
        // The Media has its own block of data.
        ASSERT_NE(m1.getData().get(), nullptr);
        // Verify that dimensions were left unchanged.
        ASSERT_EQ(m1.getData()->getDimensions(), dims);
    }
};

namespace MediaDataTests {
    TEST(DimensionsTest, ValuesRecord) {
        RecordProperty("description", "Are indices correctly stored and processed?");
        
        size_t width     = 240;
        size_t height    = 144;
        bit_depth bd     = bit_depth::FLOAT;
        size_t nChannels = 2;
        size_t nSlices   = 3;
        size_t nFrames   = 4;

        Dimensions dims(width, height, bd, nChannels, nSlices, nFrames);
        Dimensions dim2(width, height, bd, nChannels, nSlices, nFrames);
        Dimensions dim3 = dim2;
        
        EXPECT_EQ(dims.width, width);
        EXPECT_EQ(dims.height, height);
        EXPECT_EQ(dims.depth, bd);
        EXPECT_EQ(dims.nChannels, nChannels);
        EXPECT_EQ(dims.nSlices, nSlices);
        EXPECT_EQ(dims.nFrames, nFrames);

        EXPECT_EQ(dims, dim2);
        EXPECT_EQ(dim2, dim3);

        // 97th column from the 151st row in the 2nd slice of the 2nd channel from the 3rd frame.
        size_t pos = dims.indexOf(150, 96, 1, 1, 2);
        size_t gt  = (16*dims.width*dims.height)+150*dims.width+96;

        EXPECT_EQ(pos, gt);
    }

    TEST(MediaDataTest, DataConstructors) {
        RecordProperty("description", "Verifies that the constructors of all types of MediaData work.");

        Dimensions dims(240, 144, bit_depth::FLOAT, 2, 3, 4);

        MediaDataInstance m1(dims);
        ByteDataArena     m2(dims);
        ShortDataArena    m3(dims);
        FloatDataArena    m4(dims);
        RGBDataArena      m5(dims);
    }

    TEST(MediaDataTest, AccessContent) {
        RecordProperty("description", "Values are correctly placed in the data array.");

        size_t width     = 240;
        size_t height    = 144;
        bit_depth bd     = bit_depth::SHORT;
        size_t nChannels = 2;
        size_t nSlices   = 3;
        size_t nFrames   = 4;

        Dimensions dims(width, height, bd, nChannels, nSlices, nFrames);
        ByteData m = DataUtils::createByteData(dims);
        uint8_t* data = m->getRawData().data();

        size_t K     = width * height; // Nb of pixels in a canvas.
        size_t imgs  = nFrames * nSlices * nChannels; // Nb of canvas in this data container.
        
        // Building a data container in which each canvas is filled with the same value.
        for (size_t i = 0 ; i < imgs ; i++) {
            size_t fullShift = i * K;
            for (size_t j = fullShift ; j < fullShift + K ; j++) {
                data[j] = static_cast<uint16_t>(i);
            }
        }

        uint16_t val = 0;
        size_t all   = 0;
        for (size_t f = 0 ; f < nFrames ; f++) {
            for (size_t ch = 0 ; ch < nChannels ; ch++) {
                for (size_t s = 0 ; s < nSlices ; s++) {
                    for (size_t r = 0 ; r < height ; r++) {
                        for (size_t c = 0 ;  c < width ; c++) {
                            EXPECT_EQ(val, m->get(r, c, ch, s, f));
                            EXPECT_EQ(val, m->get(all));
                            EXPECT_EQ(val, m->at(r, c, ch, s, f));
                            EXPECT_EQ(val, m->at(all));
                            all++;
                        }
                    }
                    std::pair<size_t, size_t> ir = m->imageRange(ch, s, f);
                    for (size_t k = ir.first ; k < ir.second ; k++) {
                        EXPECT_EQ(data[k], val);
                    }
                    val++;
                }
            }
        }
    }
};

int main(int argc, char* argv[], char* env[]){

	testing::GTEST_FLAG(output) = "xml:./mediasTesting.xml";

    testing::InitGoogleTest();

    if (RUN_ALL_TESTS()){
        std::cerr << "At least a test failed" << std::endl;
    }

	return 0;
}


int _main_() {

    // 1. Processus normal de création d'un Media:
    Dimensions dims(240, 144);
    Media& m = MediaManager::getInstance().createMedia("the title", dims);

    return 0;
}

/*

Dans les MediaManager on peut  ajouter une méthode bool next(T& val, size_t& r, size_t& r) qui permeter d'itérer.
Un objet pourrait permettre de sécuriser le point de départ en cas de multi-threading.

*/