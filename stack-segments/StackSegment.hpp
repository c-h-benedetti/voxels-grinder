#ifndef STACK_SEGMENT_HPP_INCLUDED
#define STACK_SEGMENT_HPP_INCLUDED

#include <cstring>
#include <cinttypes>

//! The StackSegment class provides a buffer stored on the stack. Its size is represented by the template parameter.
/*!
 * This class is meant to be used in cases where the block of data can be dumped/loaded to/from the disk as the execution goes.
 * It is not NULL terminated (unless you manually add it yourself).
 * These objects can be stored in contiguous containers (e.g. std::vector) without memory spread.
 * As mentioned in the brief description, the size of this object is exactly the one passed as template parameter.
 * There is a compile-time safety check to ensure no 0-sized buffer can be created.
 * Also, there containers are not initialized with 0 or anything when they are instanciated (unless you use the constructor meant for this purpose).
 * You can read the content of this buffer without accessing its inner state. For example, using memcpy with this object is possible.
 * There is a compile time safety-check to make sure that N is a non-null positive integer.
 */

template <size_t N>
class StackSegment {

	static_assert ( N > 0, "The size of a stack segment cannot be 0");

protected:
	
	uint8_t buffer[N];

public:

	/// Does not do anything beside reserving the memory. The content is not initialized.
	StackSegment() = default;

	/// Builds the segment and initializes it with N times the \c value. (Equivalent to memset). No null character is added at the end.
	StackSegment(uint8_t c);

	/// Copy another segment into this segment. All bytes are copied, not only the used ones (and segments are not initialized at instanciation).
	StackSegment(const StackSegment<N>& b);

	/// Builds a segment from a void pointer and the number of bytes we want from it.
	StackSegment(const void* c, size_t s); 

	/// Overloading of copy constructor
	StackSegment<N>& operator=(const StackSegment<N>& b);


	/// Computes this buffer's length. It is equivalent to strlen. Stops at the first null character if one is found, or to the size of the buffer otherwise.
	size_t length() const;

	/**
	 * @brief Maximal size of the buffer.
	 * @return The capacity of this buffer, which is the value of the template parameter.
	 */
	constexpr size_t size() const { return N; }
	
	/// Sets all the bytes of the buffer to 0.
	void nullify();

	/// Overrides the content of this buffer with some other data.
	void override(const void* src, size_t s);

	/// Checks if the content of two equally sized buffers is identical.
	bool equals(const StackSegment<N>& s2) const;

	/// Copies the N bytes of this buffer into the provided array. Safety of the provided pointer is the user's responsibility.
	void copy_to(void* dest) const;

	/// Replaces the content from \p pos to N by \p r.
	void trim(size_t pos, uint8_t r=0);

	/// Seeks for the first null character and nullifies everything following it.
	void truncate();

	/**
	 * @brief Access the content of this buffer by index for writing and reading.
	 * @param i An index in the range [0, N[.
	 * @return A reference to the i-th byte stored in this buffer.
	 */
	inline uint8_t& operator[](const size_t& i) { return this->buffer[i]; }

	/**
	 * @brief Access the content of this buffer by index exclusively for reading.
	 * @param i An index in the range [0, N[.
	 * @return A copy of the i-th byte stored in this buffer.
	 */
	inline uint8_t operator[](const size_t& i) const { return this->buffer[i]; }

	/**
	 * @brief Converts the content to a const char*.
	 * Keep in mind that this method is unsafe beacause no null character is added at the end.
	 * So functions expecting a C string will probably act in an odd way.
	 * @return This buffer casted as a C string (const char*).
	 */
	inline const char* c_str() const { return (const char*)this->buffer; }

	/**
	 * @brief Converts the content to a uint8_t*.
	 * You can use the object itself instead of the result of this method.
	 * @return This buffer casted as a uint8_t.
	 */
	inline const uint8_t* data() const { return this->buffer; }


	StackSegment(StackSegment<N>&& b)               = delete;
	StackSegment<N>& operator=(StackSegment<N>&& b) = delete;
};

/// Lexicographic check that the content of two StackSegments is identical.
template <size_t N>
inline bool operator==(const StackSegment<N>& s1, const StackSegment<N>& s2) { return s1.equals(s2);}

/// Lexicographic check that the content of two StackSegments is different.
template <size_t N>
inline bool operator!=(const StackSegment<N>& s1, const StackSegment<N>& s2) { return !s1.equals(s2); }

#include "StackSegment.tpp"

#endif // STACK_SEGMENT_HPP_INCLUDED
