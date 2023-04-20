#include "MediaManager.hpp"
#include "gtest/gtest.h"


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


int main(int argc, char* argv[], char* env[]){

	testing::GTEST_FLAG(output) = "xml:./mediasTesting.xml";

    testing::InitGoogleTest();

    if (RUN_ALL_TESTS()){
        std::cerr << "At least a test failed" << std::endl;
    }

	return 0;
}


int _main_() {

    Dimensions dims1(144, 240);
    Dimensions dims2(144, 240);

    Media& m = MediaManager::getInstance().createMedia("the title", dims1);

    /* [MediaDataInstance]:
        MediaDataInstance md(); Tous les constructeurs de cette classe doivent être privés.
        Est-ce que la fonction qui permet d'instancier n'importe quel type est une de ses méthodes,
        ou fait-on une fonction indépendante ?
    */

    /*
        Il faut toutes les classes de conversion qui permettent de passer d'un type à l'autre.
        Ces convertisseurs doivent avoir des fonctions pour rescale le contenu.
        Le convertisseur de RGB à byte doit avoir des options sur l'algo de calcul du gris,
        et sur ce qu'on fait de la couche d'alpha.
    */

    // [MediaManager] Il manque une méthode pour créer un Media depuis un Data.

    /* [MediaDataInstance, MediaManager]:
        Rajouter des méthodes duplicate(). Ces méthodes doivent permettre
        de choisir la range qu'on veut dupliquer.
    */
    
    ByteData bdp = dynamic_pointer_cast<ByteDataArena>(m.getData());

    MediaData fdp(new ByteDataArena(dims2));
    m.setData(fdp);

    return 0;
}

/*

- [ ] Si on utilise directement le constructeur d'un data processor, la bit_depth sera overwritten.
      Sinon, on va faire une fonction qui prend un Dimensions et renvoie un pointeur dynamique de la depth correcte.

- [ ] On voudra des opérations sur les stacks, rajouter des frames, des channels, ...

- [ ] Dans les processors, on voudra un itérateur qui permet d'itérer sur toutes les valeurs contenues
      indépendament d'où elles se trouvent. Cet itérateur sera simplement utilisé pour les conversions
      de types et les opérations comme l'histogramme global.
      Comment implémenter cet itérateur pour le RGB ?

- [ ] La fonction nValues retourne le nombre de pixels dans le tableau: width*height*channels*frames*slices

- [ ] Il faut différencier scale et normalize dans les options de copie. On ne peut pas diviser que par la
      valeur max du type si on veut que l'output soit normalisé. Si le max contenu dans le Byte est 13, alors
      diviser par 255 ne donnera pas un output dans la range [0.0, 1.0] mais [0.0, 0.051]

*/

// Solution 1:
class DataUtils {

    // -------- Instanciators ------------------------------------------------------------

    static ByteData createByteData(const Dimensions& d) {
        return ByteData(new ByteDataArena(d));
    }

    static ShortData createShortData(const Dimensions& d) {
        return ShortData(new ShortDataArena(d));
    }

    static FloatData createFloatData(const Dimensions& d) {
        return FloatData(new FloatDataArena(d));
    }

    static RGBData createRGBData(const Dimensions& d) {
        return RGBData(new RGBDataArena(d));
    }

    static MediaData duplicate(const MediaData ori) {
        return nullptr;
    }

    template <typename T>
    static T duplicateAs(const T ori) {
        return nullptr;
    }

    // -------- From byte ----------------------------------------------------------------

    /**
     * @brief Duplicates a ByteData.
     * @param p An instance of ByteData. It remains untouched along the process.
     * @return A new instance of ByteData.
     */
    static ByteData convertToByte(ByteData p) {
        return DataUtils::duplicateAs<ByteData>(p);
    }

    /**
     * @brief Creates a new ShortData from an instance of ByteData.
     * @param p An instance of ByteData. It remains untouched along the process.
     * @return A new instance of ShortData.
     */
    static ShortData convertToShort(ByteData p) {
        Dimensions d = p->getDimensions();
        ShortData s = DataUtils::createShortData(p->getDimensions());
        const std::vector<uint8_t>&  p_data = p->getRawData();
        std::vector<uint16_t>& s_data = s->getRawData();
        size_t K = d.width * d.height;

        for (size_t f = 0 ; f < d.nFrames ; f++) {
            size_t fShift = f * d.nSlices * d.nChannels * K;
            for (size_t ch = 0 ; ch < d.nChannels ; ch++) {
                size_t chShift = ch * d.nChannels * K;
                for (size_t s = 0 ; s < d.nSlices ; s++) {
                    size_t sShift = s * K;
                    for (size_t r = 0 ; r < d.height ; r++) {
                        size_t rShift = r * d.width;
                        for (size_t c = 0 ; c < d.width ; c++) {
                            size_t idx = fShift + chShift + sShift + rShift + c;
                            s_data[idx] = static_cast<uint16_t>(p_data[idx]);
                        }
                    }
                }
            }
        }

        return s;
    }

    /**
     * @brief Creates a new FloatData from an instance of ByteData.
     * @param p An instance of ByteData. It remains untouched along the process.
     * @return A new instance of FloatData.
     */
    static FloatData convertToFloat(ByteData p) {
        Dimensions d         = p->getDimensions();
        FloatData s = DataUtils::createFloatData(p->getDimensions());
        const std::vector<uint8_t>&  p_data     = p->getRawData();
        std::vector<float>& s_data     = s->getRawData();
        size_t K             = d.width * d.height;

        for (size_t f = 0 ; f < d.nFrames ; f++) {
            size_t fShift = f * d.nSlices * d.nChannels * K;
            for (size_t ch = 0 ; ch < d.nChannels ; ch++) {
                size_t chShift = ch * d.nChannels * K;
                for (size_t s = 0 ; s < d.nSlices ; s++) {
                    size_t sShift = s * K;
                    for (size_t r = 0 ; r < d.height ; r++) {
                        size_t rShift = r * d.width;
                        for (size_t c = 0 ; c < d.width ; c++) {
                            size_t idx = fShift + chShift + sShift + rShift + c;
                            s_data[idx] = static_cast<float>(p_data[idx]);
                        }
                    }
                }
            }
        }

        return s;
    }

    /**
     * @brief Creates a new RGBData from an instance of ByteData.
     * An RGBData has a constant number of channels, which is 3.
     * Hence, only images with 1 to 3 channels can be casted to this type.
     * Also the bit depth per channel is set to 8 and cannot be changed.
     * Contrary to other types of images, RGB has their 3 channels interlaced.
     * - Your image can have up to 3 channels, associated channel in the RGB will be filled.
     * - Empty channels are left black.
     * - If the input Data has more than 3 channels, an error will occur.
     * @param p An instance of ByteData. It remains untouched along the process.
     * @return A new instance of FloatData.
     */
    static RGBData convertToRGB(ByteData p) {
        Dimensions d = p->getDimensions();

        if (d.nChannels > 3) {
            throw "Too many channels in input Media";
        }

        RGBData s = DataUtils::createRGBData(p->getDimensions());
        const std::vector<uint8_t>&  p_data = p->getRawData();
        std::vector<uint8_t>& s_data = s->getRawData();
        size_t K = 3 * d.width * d.height;
        size_t Q = d.width * d.height;

        for (size_t f = 0 ; f < d.nFrames ; f++) {
            size_t fShift = f * d.nSlices * K;
            for (size_t s = 0 ; s < d.nSlices ; s++) {
                size_t sShift = s * K;
                for (size_t r = 0 ; r < d.height ; r++) {
                    size_t rShift = r * d.width * 3;
                    for (size_t c = 0 ; c < d.width ; c++) {
                        size_t cShift = 3 * c;
                        for (size_t ch = 0 ; ch < d.nChannels ; ch++) {
                            size_t dstIdx = fShift + sShift + rShift + cShift + ch;
                            size_t srcIdx = (f * d.nSlices * d.nChannels * Q) + (ch * d.nChannels * Q) + (s * Q) + (r * d.width) + c;
                            s_data[dstIdx] = p_data[srcIdx];
                        }
                    }
                }
            }
        }
        
        return s;
    }

    // -------- From short ---------------------------------------------------------------

    static ByteData convertToByte(ShortData p) {
        ByteData s = DataUtils::createByteData(p->getDimensions());
        return s;
    }

    static ShortData convertToShort(ShortData p) {
        return DataUtils::duplicateAs<ShortData>(p);
    }

    static FloatData convertToFloat(ShortData p) {
        FloatData s = DataUtils::createFloatData(p->getDimensions());
        return s;
    }

    static RGBData convertToRGB(ShortData p) {
        RGBData s = DataUtils::createRGBData(p->getDimensions());
        return s;
    }

    // -------- From float ---------------------------------------------------------------

    static ByteData convertToByte(FloatData p) {
        ByteData s = DataUtils::createByteData(p->getDimensions());
        return s;
    }

    static ShortData convertToShort(FloatData p) {
        ShortData s = DataUtils::createShortData(p->getDimensions());
        return s;
    }

    static FloatData convertToFloat(FloatData p) {
        return DataUtils::duplicateAs<FloatData>(p);
    }

    static RGBData convertToRGB(FloatData p) {
        RGBData s = DataUtils::createRGBData(p->getDimensions());
        return s;
    }

    // -------- From RGB -----------------------------------------------------------------

    static ByteData convertToByte(RGBData p) {
        ByteData s = DataUtils::createByteData(p->getDimensions());
        return s;
    }

    static ShortData convertToShort(RGBData p) {
        ShortData s = DataUtils::createShortData(p->getDimensions());
        return s;
    }

    static FloatData convertToFloat(RGBData p) {
        FloatData s = DataUtils::createFloatData(p->getDimensions());
        return s;
    }

    static RGBData convertToRGB(RGBData p) {
        return DataUtils::duplicateAs<RGBData>(p);
    }
};


/*
____ SOLUTION 1 _________________________________________

(+):
    - On peut facilement rajouter des options individuelles à chaque opération pour changer
      d'algo ou modifier le résultat.

(-):
    - C'est très verbeux et potentiellement compliqué à maintenir si le code est presque similaire
      pour un même type cible.

____ SOLUTION 2 _________________________________________

(+):

(-):

*/

/*

Note:

    nFrames   = 10
    nSlices   = 30
    nChannels = 5
    width     = 2048
    height    = 2048
    (Byte)    => 6.3 Go
    (Float)   => 25.2 Go

    nFrames   = 1
    nSlices   = 10
    nChannels = 4
    width     = 2048
    height    = 2048
    (Byte)    => 0.2 Go
    (Float)   => 0.8 Go
*/