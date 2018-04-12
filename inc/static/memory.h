/** @file    memory.h
 *  @brief   This header is a part of the memory management library.
 *  @ingroup memory_management
 */

#ifndef STATIC_MEMORY_H_
#define STATIC_MEMORY_H_

#include "iterator.h"

namespace sstd {

/** @brief  Copies elements from the range [first, last] to an uninitialized
 *          memory area beginning at d_first.
 *  @tparam InputIt  The iterator type of the range to be copied.
 *  @tparam ForwardIt  The iterator type of the destination buffer.
 *  @param  first  The begin of the range to be copied.
 *  @param  last  The end of the range to be copied.
 *  @param  d_first  The begin of the destination range.
 */
template<class InputIt, class ForwardIt>
ForwardIt uninitialized_copy(InputIt first, InputIt last, ForwardIt d_first) {
	typedef typename iterator_traits<ForwardIt>::value_type value_type;

	for (; first != last; ++first, ++d_first) {
		new (static_cast<void*>(&*d_first)) value_type(*first);
	}

	return d_first;
}

/** @brief  Copies count elements from a range beginning at first to an
 *          uninitialized memory area beginning at d_first.
 *  @tparam InputIt  The iterator type of the range to be copied.
 *  @tparam Size  The size type of the number of elements.
 *  @tparam ForwardIt  The iterator type of the destination buffer.
 *  @param  first  The begin of the range to be copied.
 *  @param  count  The number of elements to copy.
 *  @param  d_first  The begin of the destination range.
 */
template<class InputIt, class Size, class ForwardIt>
ForwardIt uninitialized_copy_n(InputIt first, Size count, ForwardIt d_first) {
	typedef typename iterator_traits<ForwardIt>::value_type value_type;

	for (; count; ++first, ++d_first, --count) {
		new (static_cast<void*>(&*d_first)) value_type(*first);
	}

	return d_first;
}

/** @brief  Copies the given value to an uninitialized memory area, defined by
 *          the range [first, last].
 *  @tparam ForwardIt  The iterator type of the destination buffer.
 *  @tparam T  The type of the value being copied.
 *  @param  first  The begin of the range to be copied.
 *  @param  last  The end of the range to be copied.
 *  @param  value  The value to copy into the range.
 */
template<class ForwardIt, class T>
void uninitialized_fill(ForwardIt first, ForwardIt last, const T& value) {
	typedef typename iterator_traits<ForwardIt>::value_type value_type;

	for (; first != last; ++first) {
		new (static_cast<void*>(&*first)) value_type(value);
	}
}

/** @brief  Copies the given value value to the first count elements in an
 *          uninitialized memory area beginning at first.
 *  @tparam ForwardIt  The iterator type of the destination buffer.
 *  @tparam Size  The size type of the number of elements.
 *  @tparam T  The type of the value being copied.
 *  @param  first  The begin of the range to be copied.
 *  @param  count  The number of elements to copy.
 *  @param  value  The value to copy into the range.
 */
template<class ForwardIt, class Size, class T>
ForwardIt uninitialized_fill_n(ForwardIt first, Size count, const T& value) {
	typedef typename iterator_traits<ForwardIt>::value_type value_type;

	for (; count; ++first, --count) {
		new (static_cast<void*>(&*first)) value_type(value);
	}

	return first;
}

/** @brief  Constructs objects in the uninitialized storage designated by the
 *          range [first, last] by default-initialization.
 *  @tparam ForwardIt  Iterator type of the range to construct.
 *  @param  first  The begin of the range being constructed.
 *  @param  last  The end of the range being constructed.
 */
template<class ForwardIt>
void uninitialized_default_construct(ForwardIt first, ForwardIt last) {
	typedef typename iterator_traits<ForwardIt>::value_type value_type;

	for (; first != last; ++first) {
		new (static_cast<void*>(&*first)) value_type;
	}
}

/** @brief  Constructs n objects in the uninitialized storage starting at first
 *          by default-initialization.
 *  @tparam ForwardIt  Iterator type of the range to construct.
 *  @tparam Size  The size type of the range.
 *  @param  first  The begin of the range being constructed.
 *  @param  n  The number of elements to construct.
 */
template<class ForwardIt, class Size>
ForwardIt uninitialized_default_construct_n(ForwardIt first, Size n) {
	typedef typename iterator_traits<ForwardIt>::value_type value_type;

	for (; n; ++first, --n) { new (static_cast<void*>(&*first)) value_type; }

	return first;
}

/** @brief  Constructs objects in the uninitialized storage designated by the
 *          range [first, last] by value-initialization.
 *  @tparam ForwardIt  Iterator type of the range to construct.
 *  @param  first  The begin of the range being constructed.
 *  @param  last  The end of the range being constructed.
 */
template<class ForwardIt>
void uninitialized_value_construct(ForwardIt first, ForwardIt last) {
	typedef typename iterator_traits<ForwardIt>::value_type value_type;

	for (; first != last; ++first) {
		new (static_cast<void*>(&*first)) value_type();
	}
}

/** @brief  Constructs n objects in the uninitialized storage starting at first
 *          by value-initialization.
 *  @tparam ForwardIt  Iterator type of the range to construct.
 *  @tparam Size  The size type of the range.
 *  @param  first  The begin of the range being constructed.
 *  @param  n  The number of elements to construct.
 */
template<class ForwardIt, class Size>
ForwardIt uninitialized_value_construct_n(ForwardIt first, Size n) {
	typedef typename iterator_traits<ForwardIt>::value_type value_type;

	for (; n; ++first, --n) { new (static_cast<void*>(&*first)) value_type(); }

	return first;
}

/** @brief  Calls the destructor of the object pointed to by p as if by p->~T().
 *  @tparam T  Type of object to destroy.
 *  @param  p  Pointer to object being destroyed.
 */
template<class T>
void destroy_at(T* p) {
	p->~T();
}

/** @brief  Destroys the objects in the range [first, last].
 *  @tparam ForwardIt  ForwardIterator type of the supplied range.
 *  @param  first  The begin of the range to destroy.
 *  @param  last  The end of the range to destroy.
 */
template<class ForwardIt>
void destroy(ForwardIt first, ForwardIt last) {
	for (; first != last; ++first) { destroy_at(&*first); }
}

/** @brief  Destroys the n objects in the range starting at first.
 *  @tparam ForwardIt  ForwardIterator type of the supplied range.
 *  @tparam Size  Number of contiguous items to destroy.
 *  @param  first  Iterator to the first element to destroy.
 *  @param  n  Number of elements to destroy.
 */
template<class ForwardIt, class Size>
ForwardIt destroy_n(ForwardIt first, Size n) {
	for (; n; ++first, --n) { destroy_at(&*first); }

	return first;
}

} /* namespace sstd */

#endif /* STATIC_MEMORY_H_ */
