#include <iostream>
#include "gtest/gtest.h"

#define UTEST_ROOT "../testing-data/"

TEST(BucketBehavior, CheckRange) {
    RecordProperty("description", "Is the range represented by a Bucket necessarily valid?");
}


TEST(IOBehavior, FromDisk) {
    RecordProperty("description", "Can we open every data types with a generic function?");
}


TEST(TaskUsage, FromCore) {
    RecordProperty("description", "Verifies that the architecture of a task allows to use it conveniently from C++.");
}


TEST(TaskUsage, FromAction) {
    RecordProperty("description", "Verifies that the architecture of a task allows to use it conveniently from a generic Action.");
}


TEST(BehaviorTests, Step1) {
    RecordProperty("description", "Code that must run to validate the first batch of objectives.");
}


int main(int argc, char* argv[], char* env[]) {
    ::testing::GTEST_FLAG(output) = "xml:./core_tests.xml";
    ::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}

/** OBJECTIVE: [09 Dec. 2023]
 * 
 * 1. Pouvoir lancer le soft avec un `main()` qui exécute des instructions de base.
 * 2. Pouvoir créer une image, y écrire et y lire des valeurs.
 * 3. Pouvoir appliquer un threshold en multi-threads à l'image.
 * 4. Pouvoir extraire des metadata depuis un fichier PGM.
 * 5. Pouvoir lire et écrire des images au format PGM.
 */
