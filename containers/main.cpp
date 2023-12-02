#include <iostream>
#include "gtest/gtest.h"

#include "VoxelsCanvas.hpp"
#include "GeometryCanvas.hpp"


TEST(Behavior, B1) {
    RecordProperty("description", "Instanciate a canvas, filled with values and get its statistics.");

}


int main(int argc, char* argv[], char* env[]) {

    testing::GTEST_FLAG(output) = "xml:./dataContainersTests.xml";

    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}

/** TODO: Tests unitaires
 * 
 * - [ ] Chaque type de conteneur a un dtype unique.
 * - [ ] Tous les type de canvas connus répondent à is_voxels_canvas(), et pas les autres.
 * - [ ] Les données sont stockées correctement dans le canvas.
 *
**/


/** TODO: Implémentation
 *
 * - [ ] Une façon opti d'accéder aux valeurs stockées dans les canvas.
 *       Une telle fonction pourrait prendre un Bucket et retourner les paires d'indices les plus optis?
 * - [ ] Pouvoir dump et load à la volée depuis le disque.
 *
**/