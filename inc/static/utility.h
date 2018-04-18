/** @file    utility.h
 *  @brief   This header is a part of the utilities library.
 *  @ingroup general_purpose
 */

#ifndef STATIC_UTILITY_H_
#define STATIC_UTILITY_H_

namespace sstd {

/** @brief  Stores two heterogeneous objects as a single unit.
 *  @tparam T1  The type of the first value.
 *  @tparam T2  The type of the second value.
 *
 * Implements a binary tuple. A pair is a struct template that stores two
 * distinct objects as a single type.
 */
template<class T1, class T2>
struct pair {
	typedef T1 first_type;
	typedef T2 second_type;

	/** @brief Default construct. */
	pair() : first(first_type()), second(second_type()) {}

	/** @brief Initializes first with x and second with y.
	 *  @param x  The value to copy into first.
	 *  @param y  The value to copy into second.
	 */
	pair(const first_type& x, const second_type& y) : first(x), second(y) {}

	/** @brief  Construct from a pair with compatible types.
	 *  @tparam U1  Type which is compatible with T1.
	 *  @tparam U2  Type which is compatible with T2.
	 *  @param  other  The pair with compatible types being copied.
	 */
	template<class U1, class U2>
	pair(const pair<U1, U2>& other) :
	  first(other.first),
	  second(other.second) {}

	/** @brief  Assign from a pair with compatible types.
	 *  @tparam U1  Type which is compatible with T1.
	 *  @tparam U2  Type which is compatible with T2.
	 *  @param  other  The pair with compatible types being copied.
	 */
	template<class U1, class U2>
	pair& operator=(const pair<U1, U2>& other) {
		first  = other.first;
		second = other.second;

		return *this;
	}

	first_type first;
	second_type second;
};

/** @brief  Creates an sstd::pair object, deducing the target type from the
 * types of arguments.
 *  @tparam T1  The type of the first value.
 *  @tparam T2  The type of the second value.
 *  @param  t  The value to initialize first with.
 *  @param  u  The value to initialize second with.
 */
template<class T1, class T2>
inline pair<T1, T2> make_pair(T1 t, T2 u) {
	return pair<T1, T2>(t, u);
}

} /* namespace sstd */

#endif /* STATIC_UTILITY_H_ */
