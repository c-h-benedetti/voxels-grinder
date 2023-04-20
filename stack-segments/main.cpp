#include <iostream>
#include <map>
#include "ID.hpp"
#include "StackSegment.hpp"
#include "LUT_IDs.h"
#include "gtest/gtest.h"


TEST(StackSegment, NumberOfBytes) {
    RecordProperty("description", "Is an instance exactly the specified size? (no padding)");
    
	StackSegment<1> sz1;
	StackSegment<2> sz2;
	StackSegment<256> sz256;
	StackSegment<13> sz13;
	StackSegment<551> sz551;

	EXPECT_EQ(sizeof(sz1), 1);
	EXPECT_EQ(sizeof(sz2), 2);
	EXPECT_EQ(sizeof(sz256), 256);
	EXPECT_EQ(sizeof(sz13), 13);
	EXPECT_EQ(sizeof(sz551), 551);
}


TEST(StackSegment, NoInitialization) {
    RecordProperty("description", "Is a segment left uninitialzed when the default constructor is used?");
    
	constexpr char test[] = "abcdefghijklmnopqrstuvwxyz";
	constexpr size_t size = sizeof(test);
	bool go = true;

	while (go) {
		StackSegment<size> s1(test, size);
		go = false;
	}

	// The space used by s1 from the loop should have been released and left untouched from here.
	StackSegment<size> s2;
	// The space allocated to s2 should be the one previously owned by s1 (that doesn't exist anymore)
	for (size_t i = 0 ; i < size ; i++){
		EXPECT_EQ(s2[i], test[i]);
	}
}


TEST(StackSegment, FillingConstructor) {
    RecordProperty("description", "Is the data filled with the same value all the way?");

	constexpr size_t size = 32;
	constexpr uint8_t val = -126;

	StackSegment<size> st_null(0);
	StackSegment<size> st_zero('0');
	// StackSegment<size> st_neg(-126);  >>> Won't work !! Value must be stored or casted since we use unsigned!
	StackSegment<size> st_neg(val);

	for (size_t i = 0 ; i < size ; i++){
		EXPECT_EQ(st_null[i], 0);
		EXPECT_EQ(st_zero[i], '0');
		// EXPECT_EQ(st_neg[i], -126);
		EXPECT_EQ(st_neg[i], val);
	}
}


TEST(StackSegment, CopyConstructor){
	RecordProperty("description", "Is the copy constructor working?");

	constexpr char test[] = "abcdefghijklmnopqrstuvwxyz";
	constexpr size_t size = sizeof(test);

	StackSegment<size> s1(test, size);
	StackSegment<size> s2(s1);
	StackSegment<size> s3 = s2;
	
	for (size_t i = 0 ; i <size ; i++){
		EXPECT_EQ(s1[i], s2[i]);
		EXPECT_EQ(s3[i], s2[i]);
	}
}


TEST(StackSegment, CopyDataConstructor) {
    RecordProperty("description", "Is the constructor copying a segment of memory working?");

	constexpr char test[] = "abcdef";

	StackSegment<sizeof(test)> stsg(test, sizeof(test));

	for (size_t i = 0 ; i < sizeof(test) ; i++){
		EXPECT_EQ(stsg[i], test[i]);
	}
}


TEST(StackSegment, CopyOverflow) {
    RecordProperty("description", "Is the copying method safe in case of excessive data's length?");

	constexpr char test[] = "abcdefghijklmnopqrstuvwxyz";
	constexpr size_t size = sizeof(test);

	StackSegment<size-5> s1(test, size);

	for (size_t i = 0 ; i < size-5 ; i++){
		EXPECT_EQ(test[i], s1[i]);
	}
}


TEST(StackSegment, LengthTest) {
    RecordProperty("description", "Is the value returned by the `length` method correct?");

	constexpr char t1[] = "abcdefghijklmnopqrstuvwxyz";
	constexpr char t2[] = "abcdefghijklmnop";
	constexpr char t3[] = "abcde\0fghijklmnop";

	StackSegment<sizeof(t1)> s1(t1, sizeof(t1));
	StackSegment<sizeof(t2)> s2(t2, sizeof(t2));
	StackSegment<sizeof(t2)> s3(t1, sizeof(t1));
	StackSegment<sizeof(t2)> s4(t2, sizeof(t1));
	StackSegment<sizeof(t1)> s5(t3, sizeof(t3));

	// If the segment is filled with a data shorter than its max size:
	//   - if this data contains null, it will be the breaking point.
	//   - else, the value of length is unpredictable.

	EXPECT_EQ(s1.length(), sizeof(t1)-1);
	EXPECT_EQ(s2.length(), sizeof(t2)-1);
	EXPECT_EQ(s3.length(), sizeof(t2));
	EXPECT_EQ(s4.length(), sizeof(t2)-1);
	EXPECT_EQ(s5.length(), 5);
}



TEST(StackSegment, NullifyTest) {
    RecordProperty("description", "Do instances contain only zeros after the call to `nullify`?");

	constexpr char t1[] = "abcdefghijklmnopqrstuvwxyz";

	StackSegment<sizeof(t1)> s1(t1, sizeof(t1));
	StackSegment<sizeof(t1)> s2;

	s1.nullify();
	s2.nullify();

	for (size_t i = 0 ; i < sizeof(t1) ; i++){
		EXPECT_EQ(s1[i], 0);
		EXPECT_EQ(s2[i], 0);
	}
}


TEST(StackSegment, EqualityTest) {
    RecordProperty("description", "Does the method testing for equality of buffers work?");

	constexpr char t1[] = "abcdefghijklmnopqrstuvwxyz";
	constexpr char t2[] = "abcdefghijklmnopq---------";

	// If the buffers are not completely filled (or nullified before use), the value is unpredictable.

	StackSegment<sizeof(t1)> s1(t1, sizeof(t1));
	StackSegment<sizeof(t2)> s2(t2, sizeof(t2));
	StackSegment<sizeof(t1)> s3(t1, sizeof(t1));
	
	EXPECT_EQ(s1.equals(s2), false);
	EXPECT_EQ(s1.equals(s3), true);
}


TEST(StackSegment, CopyTest) {
    RecordProperty("description", "Can an instance correctly dump its content in an array?");

	constexpr char t1[] = "abcdefghijklmnopqrstuvwxyz";
	char output2[sizeof(t1)] = {};

	StackSegment<sizeof(t1)> s1(t1, sizeof(t1));

	s1.copy_to(output2);

	for (size_t i = 0 ; i < sizeof(t1) ; i++){
		EXPECT_EQ(t1[i], output2[i]);
	}
}


TEST(StackSegment, TrimingTest) {
    RecordProperty("description", "Can the tail of buffers be correctly trimmed?");

	constexpr char t1[] = "abcdefghijklmnopqrstuvwxyz";
	constexpr char t2[] = "abcdefghijk\0lmnopqrstuvwxy";

	StackSegment<sizeof(t1)> s1(t1, sizeof(t1));
	StackSegment<sizeof(t2)> s2(t2, sizeof(t2));

	s1.trim(11, '\0');
	s2.truncate();

	for (size_t i = 0 ; i < sizeof(t1) ; i++){
		EXPECT_EQ(s1[i], s2[i]);
	}
}


/* #-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-# */

// Based on Chi-squared test
TEST(ID, IDContent) {
    RecordProperty("description", "Verifies that the distribution is uniform");

	constexpr size_t drawings    = 150000;
	constexpr size_t total       = drawings * SIZE_OF_ID; // Number of drew characters
	double expected              = double(total)/double(SIZE_OF_ID_LUT); // How many occurences are expected in each slot
	double chiSquaredSum         = 0.0;
	std::map<uint8_t, double> count;

	for (size_t i = 0 ; i < SIZE_OF_ID_LUT ; i++) {
		count[LUT_IDs[i]] = 0.0;
	}

	// Counting occurences of each char
	for (size_t i = 0 ; i < drawings ; i++){
		ID id = ID::makeID();
		for (size_t j = 0 ; j < SIZE_OF_ID ; j++){
			count[id[j]] += 1.0;
		}
	}

	// Summing compo expected/observed
	double e = expected;
	for (size_t i = 0 ; i < SIZE_OF_ID_LUT ; i++) {
		double o = count[LUT_IDs[i]];
		chiSquaredSum += ((o-e) * (o-e)) / e;
	}

	RecordProperty("chi_squared_id", "Sum of chi-squared differences: " + std::to_string(chiSquaredSum) + ". (for " + std::to_string(SIZE_OF_ID_LUT) + " possible values)");
	EXPECT_LT(chiSquaredSum, 2*(SIZE_OF_ID_LUT-1)); // Quite large tolerance, but okay for what we do.
}


TEST(ID, IDCopy) {
	RecordProperty("description", "Verifies that copying an ID results in a valid destination object.");
	ID id1 = ID::makeID();
	ID id2(id1);

	EXPECT_NE(id1.c_str(), id2.c_str());

	for (size_t i = 0 ; i < SIZE_OF_ID ; i++){
		EXPECT_EQ(id1[i], id2[i]);
	}
}


TEST(ID, AllChars) {
	RecordProperty("description", "Verifies that all characters from the LUT can be reached.");
	constexpr size_t drawings = 100;
	std::map<uint8_t, int> found{};

	for (size_t i = 0 ; i < drawings ; i++){
		ID id = ID::makeID();
		for (size_t j = 0 ; j < SIZE_OF_ID ; j++){
			found[id[j]] = 1;
		}
	}

	EXPECT_EQ(found.size(), SIZE_OF_ID_LUT);
	RecordProperty("size_id_lut", "Number of chars in ID LUT: "+std::to_string(SIZE_OF_ID_LUT));
}


TEST(ID, NullID) {
	RecordProperty("description", "Is the value of ID::nullID correct.");

	ID id1 = ID::nullID();
	ID id2;
	ID id3{};

	for (size_t i = 0 ; i < SIZE_OF_ID ; i++){
		EXPECT_EQ(id1[i], 0);
		EXPECT_EQ(id2[i], 0);
		EXPECT_EQ(id3[i], 0);
	}
}

/* #-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-#-# */


int main(int argc, char* argv[], char* env[]){

	testing::GTEST_FLAG(output) = "xml:./basicBuffersTesting.xml";

    testing::InitGoogleTest();

    if (RUN_ALL_TESTS()){
        std::cerr << "At least a test failed" << std::endl;
    }

	return 0;
}
