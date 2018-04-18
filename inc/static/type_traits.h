/** @file    type_traits.h
 *  @brief   This header is a part of the language support library.
 *  @ingroup language_support
 */

#ifndef STATIC_TYPE_TRAITS_H_
#define STATIC_TYPE_TRAITS_H_

#include <stddef.h>
#include <stdint.h>

namespace sstd {

/** @brief  Wraps a static constant of specified type.
 *  @tparam T  The type of the contained constant.
 *  @tparam v  The value of the constant.
 */
template<class T, T v>
struct integral_constant {
	static const T value = v;

	typedef T value_type;
	typedef integral_constant type;

	/** @brief Returns the wrapped value. */
	operator value_type() const { return value; }
};

typedef integral_constant<bool, true> true_type;
typedef integral_constant<bool, false> false_type;

/** @brief Provides the member typedef type which is the same as T, except that
 *         its topmost const-qualifier is removed.
 */
template<class T>
struct remove_const {
	typedef T type;
};

template<class T>
struct remove_const<const T> {
	typedef T type;
};

/** @brief Provides the member typedef type which is the same as T, except that
 *         its topmost volatile-qualifier is removed.
 */
template<class T>
struct remove_volatile {
	typedef T type;
};

template<class T>
struct remove_volatile<volatile T> {
	typedef T type;
};

/** @brief Provides the member typedef type which is the same as T, except that
 *         its topmost cv-qualifiers are removed.
 */
template<class T>
struct remove_cv {
	typedef typename remove_const<typename remove_volatile<T>::type>::type type;
};

namespace detail {

template<class>
struct is_integral : public false_type {};

template<>
struct is_integral<bool> : public true_type {};

template<>
struct is_integral<char> : public true_type {};

template<>
struct is_integral<unsigned char> : public true_type {};

template<>
struct is_integral<signed char> : public true_type {};

template<>
struct is_integral<wchar_t> : public true_type {};

template<>
struct is_integral<short> : public true_type {};

template<>
struct is_integral<unsigned short> : public true_type {};

template<>
struct is_integral<int> : public true_type {};

template<>
struct is_integral<unsigned int> : public true_type {};

template<>
struct is_integral<long> : public true_type {};

template<>
struct is_integral<unsigned long> : public true_type {};

#if __cplusplus >= 201103

template<>
struct is_integral<char16_t> : public true_type {};

template<>
struct is_integral<char32_t> : public true_type {};

template<>
struct is_integral<long long> : public true_type {};

template<>
struct is_integral<unsigned long long> : public true_type {};

#endif

} /* namespace detail */

/** @brief Checks whether T is an integral type. */
template<class T>
struct is_integral
  : public detail::is_integral<typename remove_cv<T>::type>::type {};

/** @brief If B is true, enable_if has a public member typedef type, equal to T.
 */
template<bool B, class T = void>
struct enable_if {};

template<class T>
struct enable_if<true, T> {
	typedef T type;
};

/** @brief Provides member typedef type, which is defined as T if B is true at
 *         compile time, or as F if B is false. */
template<bool B, class T, class F>
struct conditional {
	typedef T type;
};

template<class T, class F>
struct conditional<false, T, F> {
	typedef F type;
};

namespace detail {

template<class T>
struct alignment_offset {
	char pad;
	T type;
};

template<size_t Size, size_t Align>
struct alignment_min
  : public integral_constant<size_t, (Size < Align) ? Size : Align> {};

template<class T>
struct alignment_of
  : public alignment_min<sizeof(T), sizeof(alignment_offset<T>) - sizeof(T)> {};

} /* namespace detail */

/** @brief Provides the member constant value equal to the alignment requirement
 *         of the type T. */
template<class T>
struct alignment_of : public detail::alignment_of<T> {};

/** @brief  Provides the nested type type, which is a trivial type with the same
 *          alignment as Align.
 *  @tparam Align  The requested alignment of the nested type.
 */
template<size_t Align>
struct aligned_pod {
	typedef long double type;
};

template<>
struct aligned_pod<1> {
	typedef uint8_t type;
};

template<>
struct aligned_pod<2> {
	typedef uint16_t type;
};

template<>
struct aligned_pod<4> {
	typedef uint32_t type;
};

template<>
struct aligned_pod<8> {
	typedef uint64_t type;
};

/** @brief  Provides the nested type type, which is a trivial type of size N
 *          with the same alignment as Align.
 *  @tparam N  The size of raw storage, cannot be smaller than Align.
 *  @tparam Align  The requested alignment of the nested type.
 */
template<size_t N, size_t Align>
struct aligned_storage {
	union type {
		unsigned char data_[N];
		typename aligned_pod<Align>::type align_;
	};

  private:
	// This type gives compilation errors if we can't align on the requested
	// boundary for this platform.
	typedef typename enable_if<(
	   Align == alignment_of<aligned_pod<0>::type>::value ||
	   Align == alignment_of<aligned_pod<1>::type>::value ||
	   Align == alignment_of<aligned_pod<2>::type>::value ||
	   Align == alignment_of<aligned_pod<4>::type>::value ||
	   Align == alignment_of<aligned_pod<8>::type>::value)>::type
	   requested_alignment_is_not_possible_for_this_platform;

	// This type gives compilation errors if the requested size is zero.
	typedef typename enable_if<(N > 0)>::type size_must_not_be_zero;

	// This type gives compilation errors if the requested size is smaller than
	// the requested alignment.
	typedef typename enable_if<(N >= Align)>::type
	   size_must_be_greater_than_or_equal_to_alignment;
};

} /* namespace sstd */

#endif /* STATIC_TYPE_TRAITS_H_ */
