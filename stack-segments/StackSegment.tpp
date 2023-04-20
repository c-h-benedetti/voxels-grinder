
// - - - - - - - CONSTRUCTORS  - - - - - - - - - - - - - - -

/**
 * Initializes the buffer at its maximal capacity (N bytes). No extra 0 is added.
 *
 * @param c Value that will fill the buffer.
 *
 */
template <size_t N>
StackSegment<N>::StackSegment(uint8_t c) {
	memset(this->buffer, c, N);
}

template <size_t N>
StackSegment<N>::StackSegment(const StackSegment<N>& b) {
	memcpy(this->buffer, b.buffer, N);
}

/**
 * @param c A pointer to an array of data.
 * @param s Number of bytes to extract from the array \p c.
 */
template <size_t N>
StackSegment<N>::StackSegment(const void* c, size_t s) {
	if (c) this->override(c, s);
}

// - - - - - - - OPERATORS  - - - - - - - - - - - - - - -

template <size_t N>
StackSegment<N>& StackSegment<N>::operator=(const StackSegment<N>& b) {
	memcpy(this->buffer, b.buffer, N);
	return *this;
}

// - - - - - - - METHODS  - - - - - - - - - - - - - - -

/**
 * The 'length' intended here is the number of bytes you actually used within this buffer.
 * This method only makes sense if you nullified this instance before adding stuff to it.
 * It is similar in its implementation to strlen() from cstring, at the difference that there is a safety to return at most N.
 *
 * @return The length of this buffer.
 */
template <size_t N>
size_t StackSegment<N>::length() const { 
	size_t i = 0;
	while (i < N && this->buffer[i]) i++;
	return i;
}

template <size_t N>
void StackSegment<N>::nullify() {
	memset(this->buffer, 0, N);
}

/**
 * Verifies with bytewise operations that the content of both buffer is identical.
 * This functions stops either at the first different character found, or at the end of BOTH buffers.
 * It implies that if you didn't nullify your buffers, you could get unexpected outcomes from this function.
 *
 * @param s2 Another StackSegment of which we will compare the value with ourself.
 *
 * @return true if both buffer have the same content, false otherwise.
 */
template <size_t N>
bool StackSegment<N>::equals(const StackSegment<N>& s2) const {
	size_t i = 0;
	while ((i < N) && (this->buffer[i] == s2.buffer[i])) i++;
	return (i == N);
}

/**
 * Overrides the content of this buffer with the bytes found in the provided pointer.
 * There is a safety check in case the pointer is nullptr.
 * Another safety check is present, verifying that the number of bytes to copy doesn't exceed the buffer's size. If it does, the first N bytes are copied anyway.
 *
 * @param src The pointer (array) from which we will copy bytes.
 * @param s Number of bytes that we will extract from src.
 */
template <size_t N>
void StackSegment<N>::override(const void* src, size_t s) {
	if (src && s > 0) memcpy(this->buffer, src, (s > N) ? (N) : (s));
}

/**
 * Replaces all the bytes from a certain position (included) by the specified value.
 *
 * @param pos Position in the buffer from which bytes will be replaced.
 * @param r Value that will override the current content.
 */
template <size_t N>
void StackSegment<N>::trim(size_t pos, uint8_t r) {
	for (size_t i = pos ; i < N ; i++) {
		this->buffer[i] = r;
	}
}


template <size_t N>
void StackSegment<N>::truncate() {
	size_t i = 0;
	while ((i < N) && (this->buffer[i])) { i++; }
	memset(this->buffer + i, 0, N-i);
}


template <size_t N>
void StackSegment<N>::copy_to(void* dest) const {
	if (dest) memcpy(dest, this->buffer, N);
}





