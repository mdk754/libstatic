/** @file    algorithm.h
 *  @brief   This header is a part of the algorithms library.
 *  @ingroup algorithm
 */

#ifndef STATIC_ALGORITHM_H_
#define STATIC_ALGORITHM_H_

#include "iterator.h"

namespace sstd {

/** @brief  Exchanges the given values.
 *  @tparam T  Type of object to swap.
 *  @param  a  First object.
 *  @param  b  Second object.
 */
template<class T>
inline void swap(T& a, T& b) {
	T tmp(a);
	a = b;
	b = tmp;
}

/** @brief  Swaps the values of the elements the given iterators point to.
 *  @tparam ForwardIt1  First forward iterator type.
 *  @tparam ForwardIt2  Second forward iterator type, must dereference to same
 *                      type as ForwardIt1.
 *  @param  a  First object.
 *  @param  b  Second object.
 */
template<class ForwardIt1, class ForwardIt2>
inline void iter_swap(ForwardIt1 a, ForwardIt2 b) {
	swap(*a, *b);
}

/** @brief  Exchanges elements between range [first1, last1] and another range
 *          starting at first2.
 *  @tparam ForwardIt1  First forward iterator type.
 *  @tparam ForwardIt2  Second forward iterator type, must dereference to same
 *                      type as ForwardIt1.
 *  @param  first1  The begin of the first range to be swapped.
 *  @param  last1  The end of the first range to be swapped.
 *  @param  first2  The begin of the second range to be swapped.
 */
template<class ForwardIt1, class ForwardIt2>
inline ForwardIt2 swap_ranges(ForwardIt1 first1,
                              ForwardIt1 last1,
                              ForwardIt2 first2) {
	for (; first1 != last1; ++first1, ++first2) { iter_swap(first1, first2); }

	return first2;
}

/** @brief  Assigns the given value to the elements in the range [first, last].
 *  @tparam ForwardIt  Forward iterator type of the range being filled.
 *  @tparam T  The type of the value to copy into the range.
 *  @param  first  The begin of the range to assign.
 *  @param  last  The end of the range to assign.
 *  @param  val  The value to fill the range with.
 */
template<class ForwardIt, class T>
inline void fill(ForwardIt first, ForwardIt last, const T& val) {
	for (; first != last; ++first) { *first = val; }
}

/** @brief  Assigns the given value to the first len elements in the range
 *          beginning at first if len > 0.
 *  @tparam OutputIt  Output iterator type of the range being filled.
 *  @tparam Size  The size type of the elements.
 *  @tparam T  The type of the value to copy into the range.
 *  @param  first  The begin of the range to assign.
 *  @param  len  The number of elements in the range.
 *  @param  val  The value to fill the range with.
 */
template<class OutputIt, class Size, class T>
inline OutputIt fill_n(OutputIt first, Size len, const T& val) {
	for (; len; --len, ++first) { *first = val; }

	return first;
}

/** @brief  Copies the elements in the range, defined by [first, last], to
 *          another range beginning at dest.
 *  @tparam InputIt  Input iterator type of the range being copied.
 *  @tparam OutputIt  Output iterator type of the range being filled.
 *  @param  first  The begin of the range to copy.
 *  @param  last  The end of the range to copy.
 *  @param  dest  The begin of the destination range.
 */
template<class InputIt, class OutputIt>
inline void copy(InputIt first, InputIt last, OutputIt dest) {
	for (; first != last; ++first, ++dest) { *dest = *first; }
}

/** @brief  Copies exactly len values from the range beginning at first to the
 *          range beginning at dest, if len > 0.
 *  @tparam InputIt  Input iterator type of the range being copied.
 *  @tparam Size  The size type of the elements.
 *  @tparam OutputIt  Output iterator type of the range being filled.
 *  @param  first  The begin of the range to copy.
 *  @param  len  The number of elements in the range.
 *  @param  dest  The begin of the destination range.
 */
template<class InputIt, class Size, class OutputIt>
inline OutputIt copy_n(InputIt first, Size len, OutputIt dest) {
	for (; len; --len, ++first, ++dest) { *dest = *first; }

	return dest;
}

/** @brief  Returns the smaller of the given values.
 *  @tparam T  Type of the values to compare.
 *  @param  a  First value.
 *  @param  b  Second value.
 */
template<class T>
inline const T& min(const T& a, const T& b) {
	return b < a ? b : a;
}

/** @brief  Returns the smaller of the given values, via comparison functor.
 *  @tparam T  Type of the values to compare.
 *  @tparam Compare  Binary comparison functor returns true if a < b.
 *  @param  a  First value.
 *  @param  b  Second value.
 *  @param  comp  Comparison functor.
 */
template<class T, class Compare>
inline const T& min(const T& a, const T& b, Compare comp) {
	return comp(b, a) ? b : a;
}

/** @brief  Returns the greater of the given values.
 *  @tparam T  Type of the values to compare.
 *  @param  a  First value.
 *  @param  b  Second value.
 */
template<class T>
inline const T& max(const T& a, const T& b) {
	return a < b ? b : a;
}

/** @brief  Returns the greater of the given values, via comparison functor.
 *  @tparam T  Type of the values to compare.
 *  @tparam Compare  Binary comparison functor returns true if a < b.
 *  @param  a  First value.
 *  @param  b  Second value.
 *  @param  comp  Comparison functor.
 */
template<class T, class Compare>
inline const T& max(const T& a, const T& b, Compare comp) {
	return comp(a, b) ? b : a;
}

/** @brief  Returns true if the range [first1, last1] is equal to the range
 *          beginning at first2.
 *  @tparam InputIt1  Must meet the requirements of InputIterator.
 *  @tparam InputIt2  Must meet the requirements of InputIterator.
 */
template<class InputIt1, class InputIt2>
bool equal(InputIt1 first1, InputIt1 last1, InputIt2 first2) {
	for (; first1 != last1; ++first1, ++first2) {
		if (!(*first1 == *first2)) { return false; }
	}

	return true;
}

/** @brief  Returns true if the range [first1, last1] is equal to the range
 *          beginning at first2, via binary predicate.
 *  @tparam InputIt1  Must meet the requirements of InputIterator.
 *  @tparam InputIt2  Must meet the requirements of InputIterator.
 *  @tparam BinaryPredicate  Binary predicate which returns true if the elements
 *                           are equal.
 */
template<class InputIt1, class InputIt2, class BinaryPredicate>
bool equal(InputIt1 first1,
           InputIt1 last1,
           InputIt2 first2,
           BinaryPredicate p) {
	for (; first1 != last1; ++first1, ++first2) {
		if (!p(*first1, *first2)) { return false; }
	}

	return true;
}

/** @brief  Checks if the first range [first1, last1] is lexicographically less
 *          than the second range [first2, last2].
 *  @tparam InputIt1  Must meet the requirements of InputIterator.
 *  @tparam InputIt2  Must meet the requirements of InputIterator.
 *  @param  first1  Begin of first range to examine.
 *  @param  last1  End of first range to examine.
 *  @param  first2  Begin of second range to examine.
 *  @param  last2  End of second range to examine.
 */
template<class InputIt1, class InputIt2>
bool lexicographical_compare(InputIt1 first1,
                             InputIt1 last1,
                             InputIt2 first2,
                             InputIt2 last2) {
	for (; (first1 != last1) && (first2 != last2); ++first1, ++first2) {
		if (*first1 < *first2) { return true; }

		if (*first2 < *first1) { return false; }
	}

	return (first1 == last1) && (first2 != last2);
}

/** @brief  Checks if the first range [first1, last1] is lexicographically less
 *          than the second range [first2, last2], via comparison functor.
 *  @tparam InputIt1  Must meet the requirements of InputIterator.
 *  @tparam InputIt2  Must meet the requirements of InputIterator.
 *  @tparam Compare  Binary predicate which returns true if the first argument
 *                   is less than the second.
 *  @param  first1  Begin of first range to examine.
 *  @param  last1  End of first range to examine.
 *  @param  first2  Begin of second range to examine.
 *  @param  last2  End of second range to examine.
 *  @param  comp  Comparison function object.
 */
template<class InputIt1, class InputIt2, class Compare>
bool lexicographical_compare(InputIt1 first1,
                             InputIt1 last1,
                             InputIt2 first2,
                             InputIt2 last2,
                             Compare comp) {
	for (; (first1 != last1) && (first2 != last2); ++first1, ++first2) {
		if (comp(*first1, *first2)) { return true; }

		if (comp(*first2, *first1)) { return false; }
	}

	return (first1 == last1) && (first2 != last2);
}

/** @brief  Reverses the order of the elements in the range [first, last].
 *  @tparam BidirIt  Bidirectional Iterator type of the range.
 *  @param  first  The begin of the range to be reversed.
 *  @param  last  The end of the range to be reversed.
 */
template<class BidirIt>
inline void reverse(BidirIt first, BidirIt last) {
	for (; first != last && first != --last; ++first) {
		iter_swap(first, last);
	}
}

/** @brief  Performs a left rotation on a range of elements.
 *  @tparam BidirIt  Bidirectional Iterator type of the range.
 *  @param  first  The begin of the range to be rotated.
 *  @param  n_first  The position of the new first element after the rotation.
 *  @param  last  The end of the range to be rotated.
 *
 * You can perform a right rotation by using reverse iterators in place of the
 * normal iterators.
 */
template<class BidirIt>
BidirIt rotate(BidirIt first, BidirIt n_first, BidirIt last) {
	if (first == n_first) { return last; }
	if (n_first == last) { return first; }

	reverse(first, n_first);
	reverse(n_first, last);
	reverse(first, last);

	return first + distance(n_first, last);
}

} /* namespace sstd */

#endif /* STATIC_ALGORITHM_H_ */
