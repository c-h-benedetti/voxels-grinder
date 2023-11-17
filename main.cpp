#include <iostream>
#include "gtest/gtest.h"


TEST(Behavior, B1) {
    RecordProperty("description", "Instanciate a canvas, filled with values and get its statistics.");

	constexpr size_t h = 128;
	constexpr size_t w = 128;
	constexpr size_t d = 15;
	constexpr size_t c = 3;
	constexpr size_t f = 10;
	constexpr size_t N = 3;

	VoxelsCanvasU8    vcu8(h, w, d, c, f);
	VoxelsCanvasU16   vcu16(h, w, d, c, f);
	VoxelsCanvasFloat vcf(h, w, d, c, f);

	Data data[N] = {&vcu8, &vcu16, &vcf};

	for (size_t i = 0 ; i < N ; i++) {
		CanvasStatistics cs(data[i]);
		cs.process_statistics(0, 1);
		const std::vector<Statistics>& stats = cs.get_stats_by_slice();
		EXPECT_TRUE(stats.size() == d);
		EXPECT_TRUE(stats[0].mean > 0.0f);
	}
}


int main(int argc, char* argv[], char* env[]) {

    testing::GTEST_FLAG(output) = "xml:./behavior-tests.xml";

    testing::InitGoogleTest(&argc, argv);

    return RUN_ALL_TESTS();
}