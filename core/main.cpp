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

/** OBJECTIVE: [09 Oct. 2023]
 * 
 * 1. Pouvoir lancer le soft avec un `main()` qui exécute des instructions de base.
 * 2. Pouvoir créer une image, y écrire et y lire des valeurs.
 * 3. Pouvoir appliquer un threshold en multi-threads à l'image.
 * 4. Pouvoir extraire des metadata depuis un fichier PGM.
 * 5. Pouvoir lire et écrire des images au format PGM.
 */

/** TODO: [09 Oct. 2023]
 * 
 * - [ ] Réviser le système de Bucket pour faire fonctionner au moins un algorithme de threshold.
 * - [ ] Terminer la dynamique entre les images et les proxies (pour petites images).
 */

/** NOTE:
 * 
 * L'implémentation pourrait switch à SFML plutôt que Gtk dans la mesure où on peut faire la GUI avec des objets.
 * Cela fonctionnerait de la même fonction que sur Blender en plaçant des objets sur différents Z.
 * Ca rendrait les animations beaucoup plus simples dans la mesure où on peut les exprimer en coordonnées fenêtre.
 */

/** NOTE: > Handling a file with invalid content.
 * 
 * If the file looks valid in terms of existence and extension, the proxy will be generated.
 * However, if the content of the file is invalid, it won't be detected.
 * It is only when the Data instance will request the first chunks of data that a failure will occur.
 */

 /** NOTE: > Usage of `DataProxy` objects
  * 
  * When we create a VoxelsGrid from scratch with the constructor, we have to consider that it might be too big for the RAM.
  * A Data object should be able to instanciate a Proxy (certainly a TiffProxy) to dump the excess of the disk.
  */