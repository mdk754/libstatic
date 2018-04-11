/** @file    algorithm.h
 *  @brief   This header is a part of the algorithms library.
 *  @ingroup algorithm
 */

#ifndef STATIC_ALGORITHM_H_
#define STATIC_ALGORITHM_H_

#include "iterator.h"

namespace sstd {

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

} /* namespace sstd */

#endif /* STATIC_ALGORITHM_H_ */
