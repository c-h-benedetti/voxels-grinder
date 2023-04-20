#ifndef BASIC_ID_HPP_INCLUDED
#define BASIC_ID_HPP_INCLUDED

#include <random>
#include <functional>
#include "StackSegment.hpp"

#define SIZE_OF_ID 32

//! The ID class provides a fixed sized ID.
/*!
	ID instances are composed of exactly 32 characters. This size is stored in the preprocessor variable SIZE_OF_ID.
	IDs are formed by drawing characters according to a uniform distribution from a 36-characters long LUT. Hence, we can have 36^32 different IDs (6.4E49).
	This LUT is accessible through the preprocessor variable SIZE_OF_ID_LUT.
	Using the default constructor always generate the nullID.
	IDs can be compared to each other in the lexicographic order of bytes composing it.
*/

class ID : public StackSegment<SIZE_OF_ID> {

	static std::random_device rd;
	static std::mt19937 mt;
	static std::uniform_int_distribution<uint8_t> dist;
	static std::function<uint8_t()> generator;
	
	constexpr void nullInit() {
		for (size_t i = 0 ; i < SIZE_OF_ID ; i++) {
			this->buffer[i] = 0;
		}
	}

	void randomize();
	
public:

	/// Default constructor. IDs are initialized at the nullID value.
	constexpr ID() { this->nullInit(); }

	/** @return The nullid (filled with 0). It indicates that an object is invalid.*/
	static constexpr ID nullID() { return ID{}; }
	
	/** @return A random ID.*/
	static inline ID makeID() {
		ID id;
		id.randomize(); 
		return id; 
	}

	void override(const void* src, const size_t& s)  = delete;
	void trim(const size_t& pos, const uint8_t& r=0) = delete;
	void truncate()                                  = delete;
};


inline bool operator<(const ID& id1, const ID& id2) {
	size_t i = 0;
	while ((i < SIZE_OF_ID) && (id1[i] == id2[i])) { i++; }
	if (i == SIZE_OF_ID) { return false; }
	return id1[i] < id2[i];
}


inline bool operator==(const ID& id1, const ID& id2){
	size_t i = 0;
	while ((i < SIZE_OF_ID) && (id1[i] == id2[i])) { i++; }
	return i == SIZE_OF_ID;
}

#endif // BASIC_ID_HPP_INCLUDED