/** @file    array.h
 *  @brief   This header is a part of the containers library.
 *  @ingroup sequence_container
 */

#ifndef STATIC_ARRAY_H_
#define STATIC_ARRAY_H_

#include <stddef.h>

#include "algorithm.h"
#include "iterator.h"

namespace sstd {

/** @brief  Static contiguous array.
 *  @tparam T  Type stored by the container.
 *  @tparam N  Number of elements in the container.
 *
 * A container that encapsulates fixed size arrays. This container is an
 * aggregate type with the same semantics as a struct holding a C-style array as
 * its only non-static data member. This is a direct replacement for an
 * std::array in cases where it is unavailable.
 */
template<class T, size_t N>
class array {
  public:
	typedef T value_type;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef pointer iterator;
	typedef const_pointer const_iterator;
	typedef sstd::reverse_iterator<iterator> reverse_iterator;
	typedef sstd::reverse_iterator<const_iterator> const_reverse_iterator;

	/** @brief Access specified element with bounds checking.
	 *  @param pos Position of the element within the container.
	 */
	reference at(size_type pos) { return buf_[pos % N]; }
	const_reference at(size_type pos) const { return buf_[pos % N]; }

	/** @brief Access specified element.
	 *  @param pos Position of the element within the container.
	 */
	reference operator[](size_type pos) { return buf_[pos]; }
	const_reference operator[](size_type pos) const { return buf_[pos]; }

	/** @brief Access the first element. */
	reference front() { return buf_[0]; }
	const_reference front() const { return buf_[0]; }

	/** @brief Access the last element. */
	reference back() { return buf_[N - 1]; }
	const_reference back() const { return buf_[N - 1]; }

	/** @brief Access the underlying array pointer. */
	pointer data() { return buf_; }
	const_pointer data() const { return buf_; }

	/** @brief Returns an iterator to the first element. */
	iterator begin() { return buf_; }
	const_iterator begin() const { return buf_; }
	const_iterator cbegin() const { return buf_; }

	/** @brief Returns an iterator to one past the last element. */
	iterator end() { return buf_ + N; }
	const_iterator end() const { return buf_ + N; }
	const_iterator cend() const { return buf_ + N; }

	/** @brief Returns a reverse iterator to the first element. */
	reverse_iterator rbegin() { return reverse_iterator(end()); }
	const_reverse_iterator rbegin() const {
		return const_reverse_iterator(end());
	}
	const_reverse_iterator crbegin() const { return rbegin(); }

	/** @brief Returns a reverse iterator to one past the last element. */
	reverse_iterator rend() { return reverse_iterator(begin()); }
	const_reverse_iterator rend() const {
		return const_reverse_iterator(begin());
	}
	const_reverse_iterator crend() const { return rend(); }

	/** @brief Checks whether the container has no elements. */
	bool empty() const { return false; }

	/** @brief Returns the number of elements in the container. */
	size_type size() const { return N; }

	/** @brief Returns the maximum possible number of elements. */
	size_type max_size() const { return N; }

	T buf_[N];
};

template<class T, size_t N, class T2, size_t N2>
inline bool operator==(const array<T, N>& lhs, const array<T2, N2>& rhs) {
	return lhs.size() == rhs.size() &&
	       equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
}

template<class T, size_t N, class T2, size_t N2>
inline bool operator!=(const array<T, N>& lhs, const array<T2, N2>& rhs) {
	return !(lhs == rhs);
}

template<class T, size_t N, class T2, size_t N2>
inline bool operator<(const array<T, N>& lhs, const array<T2, N2>& rhs) {
	return lexicographical_compare(
	   lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

template<class T, size_t N, class T2, size_t N2>
inline bool operator>(const array<T, N>& lhs, const array<T2, N2>& rhs) {
	return rhs < lhs;
}

template<class T, size_t N, class T2, size_t N2>
inline bool operator<=(const array<T, N>& lhs, const array<T2, N2>& rhs) {
	return !(rhs < lhs);
}

template<class T, size_t N, class T2, size_t N2>
inline bool operator>=(const array<T, N>& lhs, const array<T2, N2>& rhs) {
	return !(lhs < rhs);
}

} /* namespace sstd */

#endif /* STATIC_ARRAY_H_ */
