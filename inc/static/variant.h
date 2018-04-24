/** @file    variant.h
 *  @brief   This header is a part of the utilities library.
 *  @ingroup general_purpose
 */

#ifndef STATIC_VARIANT_H_
#define STATIC_VARIANT_H_

#include <stddef.h>
#include <stdint.h>

#include "type_traits.h"

namespace sstd {

/** @brief Placeholder type for use as the first alternative in a variant of
 *         non-default-contructible types.
 */
struct monostate {};

/** @brief This is a special value equal to the largest value representable by
 *         the type uint8_t. */
static const size_t variant_npos = static_cast<uint8_t>(-1);

namespace detail {

/** @brief This is an empty value to consume unused slots in the variant. */
template<size_t N>
struct nullvar_t {};

template<size_t N>
inline bool operator==(nullvar_t<N>, nullvar_t<N>) {
	return true;
}

template<size_t N>
inline bool operator!=(nullvar_t<N>, nullvar_t<N>) {
	return false;
}

template<size_t N>
inline bool operator<(nullvar_t<N>, nullvar_t<N>) {
	return false;
}

template<size_t N>
inline bool operator>(nullvar_t<N>, nullvar_t<N>) {
	return false;
}

template<size_t N>
inline bool operator<=(nullvar_t<N>, nullvar_t<N>) {
	return true;
}

template<size_t N>
inline bool operator>=(nullvar_t<N>, nullvar_t<N>) {
	return true;
}

} /* namespace detail */

/** @brief  Obtains the type of the alternative at the specified index.
 *  @tparam I  The index of the type to retrieve.
 *  @tparam T  The variant type to check at compile time.
 */
template<size_t I, class T>
struct variant_alternative;

/** @brief  This class template represents a type-safe union.
 *  @tparam T1  The type of the first value in the union.
 *  @tparam T2..8  The type of the 2nd through 8th value in the union.
 *
 * An instance of variant holds a value of exactly one of its alternative types
 * at any given time.
 */
template<class T0,
         class T1 = detail::nullvar_t<1>,
         class T2 = detail::nullvar_t<2>,
         class T3 = detail::nullvar_t<3>,
         class T4 = detail::nullvar_t<4>,
         class T5 = detail::nullvar_t<5>,
         class T6 = detail::nullvar_t<6>,
         class T7 = detail::nullvar_t<7> >
class variant {
	template<class T>
	struct supported_type
	  : public integral_constant<
	       bool,
	       is_same<T, T0>::value || is_same<T, T1>::value ||
	          is_same<T, T2>::value || is_same<T, T3>::value ||
	          is_same<T, T4>::value || is_same<T, T5>::value ||
	          is_same<T, T6>::value || is_same<T, T7>::value> {};

	template<class T>
	struct index_of
	  : public integral_constant<
	       size_t,
	       (is_same<T, T0>::value
	           ? 0
	           : is_same<T, T1>::value
	                ? 1
	                : is_same<T, T2>::value
	                     ? 2
	                     : is_same<T, T3>::value
	                          ? 3
	                          : is_same<T, T4>::value
	                               ? 4
	                               : is_same<T, T5>::value
	                                    ? 5
	                                    : is_same<T, T6>::value
	                                         ? 6
	                                         : is_same<T, T7>::value
	                                              ? 7
	                                              : variant_npos)> {};

	template<size_t I, class Variant>
	friend struct variant_alternative;

	template<class T,
	         class VT0,
	         class VT1,
	         class VT2,
	         class VT3,
	         class VT4,
	         class VT5,
	         class VT6,
	         class VT7>
	friend bool holds_alternative(
	   const variant<VT0, VT1, VT2, VT3, VT4, VT5, VT6, VT7>&);

	template<class R,
	         class VT0,
	         class VT1,
	         class VT2,
	         class VT3,
	         class VT4,
	         class VT5,
	         class VT6,
	         class VT7>
	friend R* get_if(variant<VT0, VT1, VT2, VT3, VT4, VT5, VT6, VT7>&);

	template<class R,
	         class VT0,
	         class VT1,
	         class VT2,
	         class VT3,
	         class VT4,
	         class VT5,
	         class VT6,
	         class VT7>
	friend const R* get_if(
	   const variant<VT0, VT1, VT2, VT3, VT4, VT5, VT6, VT7>&);

	template<class R,
	         class VT0,
	         class VT1,
	         class VT2,
	         class VT3,
	         class VT4,
	         class VT5,
	         class VT6,
	         class VT7>
	friend R& get(variant<VT0, VT1, VT2, VT3, VT4, VT5, VT6, VT7>&);

	template<class R,
	         class VT0,
	         class VT1,
	         class VT2,
	         class VT3,
	         class VT4,
	         class VT5,
	         class VT6,
	         class VT7>
	friend const R& get(const variant<VT0, VT1, VT2, VT3, VT4, VT5, VT6, VT7>&);

	template<size_t N, class = void>
	struct type_at;

	template<class Ignored>
	struct type_at<0, Ignored> {
		typedef T0 type;
	};

	template<class Ignored>
	struct type_at<1, Ignored> {
		typedef T1 type;
	};

	template<class Ignored>
	struct type_at<2, Ignored> {
		typedef T2 type;
	};

	template<class Ignored>
	struct type_at<3, Ignored> {
		typedef T3 type;
	};

	template<class Ignored>
	struct type_at<4, Ignored> {
		typedef T4 type;
	};

	template<class Ignored>
	struct type_at<5, Ignored> {
		typedef T5 type;
	};

	template<class Ignored>
	struct type_at<6, Ignored> {
		typedef T6 type;
	};

	template<class Ignored>
	struct type_at<7, Ignored> {
		typedef T7 type;
	};

	template<size_t I,
	         class VT0,
	         class VT1,
	         class VT2,
	         class VT3,
	         class VT4,
	         class VT5,
	         class VT6,
	         class VT7>
	friend typename variant<VT0, VT1, VT2, VT3, VT4, VT5, VT6, VT7>::
	   template type_at<I>::type*
	   get_if(variant<VT0, VT1, VT2, VT3, VT4, VT5, VT6, VT7>&);

	template<size_t I,
	         class VT0,
	         class VT1,
	         class VT2,
	         class VT3,
	         class VT4,
	         class VT5,
	         class VT6,
	         class VT7>
	friend const typename variant<VT0, VT1, VT2, VT3, VT4, VT5, VT6, VT7>::
	   template type_at<I>::type*
	   get_if(const variant<VT0, VT1, VT2, VT3, VT4, VT5, VT6, VT7>&);

	template<size_t I,
	         class VT0,
	         class VT1,
	         class VT2,
	         class VT3,
	         class VT4,
	         class VT5,
	         class VT6,
	         class VT7>
	friend typename variant<VT0, VT1, VT2, VT3, VT4, VT5, VT6, VT7>::
	   template type_at<I>::type&
	   get(variant<VT0, VT1, VT2, VT3, VT4, VT5, VT6, VT7>&);

	template<size_t I,
	         class VT0,
	         class VT1,
	         class VT2,
	         class VT3,
	         class VT4,
	         class VT5,
	         class VT6,
	         class VT7>
	friend const typename variant<VT0, VT1, VT2, VT3, VT4, VT5, VT6, VT7>::
	   template type_at<I>::type&
	   get(const variant<VT0, VT1, VT2, VT3, VT4, VT5, VT6, VT7>&);

  public:
	/** @brief Default construct the first variant type. */
	variant() : type_(0) { new (static_cast<void*>(&buffer_)) T0(); }

	/** @brief Copy construct. */
	variant(const variant& other) : type_(other.type_) {
		copy_construct(&other.buffer_);
	}

	/** @brief Construct from a value of the first type. */
	variant(const T0& value) : type_(0) {
		new (static_cast<void*>(&buffer_)) T0(value);
	}

	/** @brief Construct from a value of the second type. */
	variant(const T1& value) : type_(1) {
		new (static_cast<void*>(&buffer_)) T1(value);
	}

	/** @brief Construct from a value of the third type. */
	variant(const T2& value) : type_(2) {
		new (static_cast<void*>(&buffer_)) T2(value);
	}

	/** @brief Construct from a value of the fourth type. */
	variant(const T3& value) : type_(3) {
		new (static_cast<void*>(&buffer_)) T3(value);
	}

	/** @brief Construct from a value of the fifth type. */
	variant(const T4& value) : type_(4) {
		new (static_cast<void*>(&buffer_)) T4(value);
	}

	/** @brief Construct from a value of the sixth type. */
	variant(const T5& value) : type_(5) {
		new (static_cast<void*>(&buffer_)) T5(value);
	}

	/** @brief Construct from a value of the seventh type. */
	variant(const T6& value) : type_(6) {
		new (static_cast<void*>(&buffer_)) T6(value);
	}

	/** @brief Construct from a value of the eighth type. */
	variant(const T7& value) : type_(7) {
		new (static_cast<void*>(&buffer_)) T7(value);
	}

	~variant() { destroy(); }

	/** @brief Assign from another variant with matching types. */
	variant& operator=(const variant& rhs) {
		if (this == &rhs) { return *this; }

		if (type_ != rhs.type_) {
			destroy();
			type_ = rhs.type_;
			copy_construct(&rhs.buffer_);
		} else {
			copy_assign(&rhs.buffer_);
		}

		return *this;
	}

	/** @brief Assign a value of the first type into the variant. */
	variant& operator=(const T0& rhs) { return copy(rhs); }

	/** @brief Assign a value of the second type into the variant. */
	variant& operator=(const T1& rhs) { return copy(rhs); }

	/** @brief Assign a value of the third type into the variant. */
	variant& operator=(const T2& rhs) { return copy(rhs); }

	/** @brief Assign a value of the fourth type into the variant. */
	variant& operator=(const T3& rhs) { return copy(rhs); }

	/** @brief Assign a value of the fifth type into the variant. */
	variant& operator=(const T4& rhs) { return copy(rhs); }

	/** @brief Assign a value of the sixth type into the variant. */
	variant& operator=(const T5& rhs) { return copy(rhs); }

	/** @brief Assign a value of the seventh type into the variant. */
	variant& operator=(const T6& rhs) { return copy(rhs); }

	/** @brief Assign a value of the eighth type into the variant. */
	variant& operator=(const T7& rhs) { return copy(rhs); }

	/** @brief Returns the index of the type currently held by the variant. */
	size_t index() const { return type_; }

  private:
	template<class T>
	static T* as(void* data) {
		return static_cast<T*>(data);
	}

	template<class T>
	static const T* as(const void* data) {
		return static_cast<const T*>(data);
	}

	template<class T>
	variant& copy(const T& rhs) {
		if (type_ != index_of<T>::value) {
			destroy();
			type_ = index_of<T>::value;
			new (static_cast<void*>(&buffer_)) T(rhs);
		} else {
			*as<T>(&buffer_) = rhs;
		}

		return *this;
	}

	void copy_construct(const void* data) {
		switch (type_) {
			case 0:
				new (static_cast<void*>(&buffer_)) T0(*as<T0>(data));
				break;
			case 1:
				new (static_cast<void*>(&buffer_)) T1(*as<T1>(data));
				break;
			case 2:
				new (static_cast<void*>(&buffer_)) T2(*as<T2>(data));
				break;
			case 3:
				new (static_cast<void*>(&buffer_)) T3(*as<T3>(data));
				break;
			case 4:
				new (static_cast<void*>(&buffer_)) T4(*as<T4>(data));
				break;
			case 5:
				new (static_cast<void*>(&buffer_)) T5(*as<T5>(data));
				break;
			case 6:
				new (static_cast<void*>(&buffer_)) T6(*as<T6>(data));
				break;
			case 7:
				new (static_cast<void*>(&buffer_)) T7(*as<T7>(data));
				break;
		}
	}

	void copy_assign(const void* data) {
		switch (type_) {
			case 0:
				*as<T0>(&buffer_) = *as<T0>(data);
				break;
			case 1:
				*as<T1>(&buffer_) = *as<T1>(data);
				break;
			case 2:
				*as<T2>(&buffer_) = *as<T2>(data);
				break;
			case 3:
				*as<T3>(&buffer_) = *as<T3>(data);
				break;
			case 4:
				*as<T4>(&buffer_) = *as<T4>(data);
				break;
			case 5:
				*as<T5>(&buffer_) = *as<T5>(data);
				break;
			case 6:
				*as<T6>(&buffer_) = *as<T6>(data);
				break;
			case 7:
				*as<T7>(&buffer_) = *as<T7>(data);
				break;
		}
	}

	void destroy() {
		switch (type_) {
			case 0:
				as<T0>(&buffer_)->~T0();
				break;
			case 1:
				as<T1>(&buffer_)->~T1();
				break;
			case 2:
				as<T2>(&buffer_)->~T2();
				break;
			case 3:
				as<T3>(&buffer_)->~T3();
				break;
			case 4:
				as<T4>(&buffer_)->~T4();
				break;
			case 5:
				as<T5>(&buffer_)->~T5();
				break;
			case 6:
				as<T6>(&buffer_)->~T6();
				break;
			case 7:
				as<T7>(&buffer_)->~T7();
				break;
		}
	}

	template<class T>
	T* value() {
		return index_of<T>::value == type_ ? as<T>(&buffer_) : 0;
	}

	template<class T>
	const T* value() const {
		return index_of<T>::value == type_ ? as<T>(&buffer_) : 0;
	}

	template<size_t I>
	typename type_at<I>::type* value() {
		return I == type_ ? as<typename type_at<I>::type>(&buffer_) : 0;
	}

	template<size_t I>
	const typename type_at<I>::type* value() const {
		return I == type_ ? as<typename type_at<I>::type>(&buffer_) : 0;
	}

	typedef typename aligned_union<1, T0, T1, T2, T3, T4, T5, T6, T7>::type
	   aligned_buffer_type;

	aligned_buffer_type buffer_;
	uint8_t type_;
};

/** @brief  Checks if the variant holds the alternative.
 *  @tparam T  The alternative type to check the variant for.
 *  @tparam T0..7  The types of the variant, deduced from the parameter.
 *  @param  v  The variant to check for type T.
 */
template<class T,
         class T0,
         class T1,
         class T2,
         class T3,
         class T4,
         class T5,
         class T6,
         class T7>
inline bool holds_alternative(
   const variant<T0, T1, T2, T3, T4, T5, T6, T7>& v) {
	return v.type_ ==
	       variant<T0, T1, T2, T3, T4, T5, T6, T7>::template index_of<T>::value;
}

/** @brief  Obtains a pointer to the value in the variant, or null.
 *  @tparam R  The type to retrieve from the variant.
 *  @tparam T0..7  The types of the variant, deduced from the parameter.
 *  @param  v  The variant to get the value from.
 */
template<class R,
         class T0,
         class T1,
         class T2,
         class T3,
         class T4,
         class T5,
         class T6,
         class T7>
inline R* get_if(variant<T0, T1, T2, T3, T4, T5, T6, T7>& v) {
	return v.template value<R>();
}

/** @brief  Obtains a const pointer to the value in the variant, or null.
 *  @tparam R  The type to retrieve from the variant.
 *  @tparam T0..7  The types of the variant, deduced from the parameter.
 *  @param  v  The variant to get the value from.
 */
template<class R,
         class T0,
         class T1,
         class T2,
         class T3,
         class T4,
         class T5,
         class T6,
         class T7>
inline const R* get_if(const variant<T0, T1, T2, T3, T4, T5, T6, T7>& v) {
	return v.template value<R>();
}

/** @brief  Obtains a pointer to the value in the variant, or null.
 *  @tparam I  The index of the type to retrieve from the variant.
 *  @tparam T0..7  The types of the variant, deduced from the parameter.
 *  @param  v  The variant to get the value from.
 */
template<size_t I,
         class T0,
         class T1,
         class T2,
         class T3,
         class T4,
         class T5,
         class T6,
         class T7>
inline
   typename variant<T0, T1, T2, T3, T4, T5, T6, T7>::template type_at<I>::type*
   get_if(variant<T0, T1, T2, T3, T4, T5, T6, T7>& v) {
	return v.template value<I>();
}

/** @brief  Obtains a const pointer to the value in the variant, or null.
 *  @tparam I  The index of the type to retrieve from the variant.
 *  @tparam T0..7  The types of the variant, deduced from the parameter.
 *  @param  v  The variant to get the value from.
 */
template<size_t I,
         class T0,
         class T1,
         class T2,
         class T3,
         class T4,
         class T5,
         class T6,
         class T7>
inline const typename variant<T0, T1, T2, T3, T4, T5, T6, T7>::template type_at<
   I>::type*
   get_if(const variant<T0, T1, T2, T3, T4, T5, T6, T7>& v) {
	return v.template value<I>();
}

/** @brief   Obtains a reference to the value in the variant.
 *  @tparam  R  The type to retrieve from the variant.
 *  @tparam  T0..7  The types of the variant, deduced from the parameter.
 *  @param   v  The variant to get the value from.
 *  @warning This will dereference a null pointer if the type is not currently
 *           stored by the variant.
 */
template<class R,
         class T0,
         class T1,
         class T2,
         class T3,
         class T4,
         class T5,
         class T6,
         class T7>
inline R& get(variant<T0, T1, T2, T3, T4, T5, T6, T7>& v) {
	return *get_if<R>(v);
}

/** @brief   Obtains a reference to the value in the variant.
 *  @tparam  R  The type to retrieve from the variant.
 *  @tparam  T0..7  The types of the variant, deduced from the parameter.
 *  @param   v  The variant to get the value from.
 *  @warning This will dereference a null pointer if the type is not currently
 *           stored by the variant.
 */
template<class R,
         class T0,
         class T1,
         class T2,
         class T3,
         class T4,
         class T5,
         class T6,
         class T7>
inline const R& get(const variant<T0, T1, T2, T3, T4, T5, T6, T7>& v) {
	return *get_if<R>(v);
}

/** @brief   Obtains a reference to the value in the variant.
 *  @tparam  I  The index of the type to retrieve from the variant.
 *  @tparam  T0..7  The types of the variant, deduced from the parameter.
 *  @param   v  The variant to get the value from.
 *  @warning This will dereference a null pointer if the type is not currently
 *           stored by the variant.
 */
template<size_t I,
         class T0,
         class T1,
         class T2,
         class T3,
         class T4,
         class T5,
         class T6,
         class T7>
inline
   typename variant<T0, T1, T2, T3, T4, T5, T6, T7>::template type_at<I>::type&
   get(variant<T0, T1, T2, T3, T4, T5, T6, T7>& v) {
	return *get_if<I>(v);
}

/** @brief   Obtains a const reference to the value in the variant.
 *  @tparam  I  The index of the type to retrieve from the variant.
 *  @tparam  T0..7  The types of the variant, deduced from the parameter.
 *  @param   v  The variant to get the value from.
 *  @warning This will dereference a null pointer if the type is not currently
 *           stored by the variant.
 */
template<size_t I,
         class T0,
         class T1,
         class T2,
         class T3,
         class T4,
         class T5,
         class T6,
         class T7>
inline const typename variant<T0, T1, T2, T3, T4, T5, T6, T7>::template type_at<
   I>::type&
   get(const variant<T0, T1, T2, T3, T4, T5, T6, T7>& v) {
	return *get_if<I>(v);
}

/** @brief  Calls the provided functor with the arguments held by the variant.
 *  @tparam Visitor  The functor to call with the variant value.
 *  @tparam Variant  The variant type to get the value from.
 *  @param  visitor  The instance of the functor being called.
 *  @param  v  The variant to extract its value from.
 */
template<class Visitor, class Variant>
inline Variant visit(const Visitor& visitor, const Variant& v) {
	switch (v.index()) {
		case 0:
			return visitor(get<0>(v));
		case 1:
			return visitor(get<1>(v));
		case 2:
			return visitor(get<2>(v));
		case 3:
			return visitor(get<3>(v));
		case 4:
			return visitor(get<4>(v));
		case 5:
			return visitor(get<5>(v));
		case 6:
			return visitor(get<6>(v));
		case 7:
			return visitor(get<7>(v));
		default:
			return Variant();
	}
}

template<class T0,
         class T1,
         class T2,
         class T3,
         class T4,
         class T5,
         class T6,
         class T7>
inline bool operator==(const variant<T0, T1, T2, T3, T4, T5, T6, T7>& lhs,
                       const variant<T0, T1, T2, T3, T4, T5, T6, T7>& rhs) {
	if (lhs.index() != rhs.index()) { return false; }

	switch (lhs.index()) {
		case 0:
			return get<0>(lhs) == get<0>(rhs);
		case 1:
			return get<1>(lhs) == get<1>(rhs);
		case 2:
			return get<2>(lhs) == get<2>(rhs);
		case 3:
			return get<3>(lhs) == get<3>(rhs);
		case 4:
			return get<4>(lhs) == get<4>(rhs);
		case 5:
			return get<5>(lhs) == get<5>(rhs);
		case 6:
			return get<6>(lhs) == get<6>(rhs);
		case 7:
			return get<7>(lhs) == get<7>(rhs);
		default:
			return false;
	}
}

template<class T0,
         class T1,
         class T2,
         class T3,
         class T4,
         class T5,
         class T6,
         class T7>
inline bool operator!=(const variant<T0, T1, T2, T3, T4, T5, T6, T7>& lhs,
                       const variant<T0, T1, T2, T3, T4, T5, T6, T7>& rhs) {
	return !(lhs == rhs);
}

template<class T0,
         class T1,
         class T2,
         class T3,
         class T4,
         class T5,
         class T6,
         class T7>
inline bool operator<(const variant<T0, T1, T2, T3, T4, T5, T6, T7>& lhs,
                      const variant<T0, T1, T2, T3, T4, T5, T6, T7>& rhs) {
	if (lhs.index() < rhs.index()) { return true; }
	if (lhs.index() > rhs.index()) { return false; }

	switch (lhs.index()) {
		case 0:
			return get<0>(lhs) < get<0>(rhs);
		case 1:
			return get<1>(lhs) < get<1>(rhs);
		case 2:
			return get<2>(lhs) < get<2>(rhs);
		case 3:
			return get<3>(lhs) < get<3>(rhs);
		case 4:
			return get<4>(lhs) < get<4>(rhs);
		case 5:
			return get<5>(lhs) < get<5>(rhs);
		case 6:
			return get<6>(lhs) < get<6>(rhs);
		case 7:
			return get<7>(lhs) < get<7>(rhs);
		default:
			return false;
	}
}

template<class T0,
         class T1,
         class T2,
         class T3,
         class T4,
         class T5,
         class T6,
         class T7>
inline bool operator>(const variant<T0, T1, T2, T3, T4, T5, T6, T7>& lhs,
                      const variant<T0, T1, T2, T3, T4, T5, T6, T7>& rhs) {
	return rhs < lhs;
}

template<class T0,
         class T1,
         class T2,
         class T3,
         class T4,
         class T5,
         class T6,
         class T7>
inline bool operator<=(const variant<T0, T1, T2, T3, T4, T5, T6, T7>& lhs,
                       const variant<T0, T1, T2, T3, T4, T5, T6, T7>& rhs) {
	return !(rhs < lhs);
}

template<class T0,
         class T1,
         class T2,
         class T3,
         class T4,
         class T5,
         class T6,
         class T7>
inline bool operator>=(const variant<T0, T1, T2, T3, T4, T5, T6, T7>& lhs,
                       const variant<T0, T1, T2, T3, T4, T5, T6, T7>& rhs) {
	return !(lhs < rhs);
}

/** @brief  Obtains the size of the variant's list of alternatives.
 *  @tparam T  The variant to get the size from.
 */
template<class T>
struct variant_size;

template<class T0,
         class T1,
         class T2,
         class T3,
         class T4,
         class T5,
         class T6,
         class T7>
struct variant_size<variant<T0, T1, T2, T3, T4, T5, T6, T7> >
  : public integral_constant<size_t, 8> {};

template<class T0, class T1, class T2, class T3, class T4, class T5, class T6>
struct variant_size<variant<T0, T1, T2, T3, T4, T5, T6, detail::nullvar_t<7> > >
  : public integral_constant<size_t, 7> {};

template<class T0, class T1, class T2, class T3, class T4, class T5>
struct variant_size<variant<T0,
                            T1,
                            T2,
                            T3,
                            T4,
                            T5,
                            detail::nullvar_t<6>,
                            detail::nullvar_t<7> > >
  : public integral_constant<size_t, 6> {};

template<class T0, class T1, class T2, class T3, class T4>
struct variant_size<variant<T0,
                            T1,
                            T2,
                            T3,
                            T4,
                            detail::nullvar_t<5>,
                            detail::nullvar_t<6>,
                            detail::nullvar_t<7> > >
  : public integral_constant<size_t, 5> {};

template<class T0, class T1, class T2, class T3>
struct variant_size<variant<T0,
                            T1,
                            T2,
                            T3,
                            detail::nullvar_t<4>,
                            detail::nullvar_t<5>,
                            detail::nullvar_t<6>,
                            detail::nullvar_t<7> > >
  : public integral_constant<size_t, 4> {};

template<class T0, class T1, class T2>
struct variant_size<variant<T0,
                            T1,
                            T2,
                            detail::nullvar_t<3>,
                            detail::nullvar_t<4>,
                            detail::nullvar_t<5>,
                            detail::nullvar_t<6>,
                            detail::nullvar_t<7> > >
  : public integral_constant<size_t, 3> {};

template<class T0, class T1>
struct variant_size<variant<T0,
                            T1,
                            detail::nullvar_t<2>,
                            detail::nullvar_t<3>,
                            detail::nullvar_t<4>,
                            detail::nullvar_t<5>,
                            detail::nullvar_t<6>,
                            detail::nullvar_t<7> > >
  : public integral_constant<size_t, 2> {};

template<class T0>
struct variant_size<variant<T0,
                            detail::nullvar_t<1>,
                            detail::nullvar_t<2>,
                            detail::nullvar_t<3>,
                            detail::nullvar_t<4>,
                            detail::nullvar_t<5>,
                            detail::nullvar_t<6>,
                            detail::nullvar_t<7> > >
  : public integral_constant<size_t, 1> {};

template<class T>
struct variant_size<const T> : public variant_size<T> {};

template<class T>
struct variant_size<volatile T> : public variant_size<T> {};

template<class T>
struct variant_size<const volatile T> : public variant_size<T> {};

template<size_t I,
         class T0,
         class T1,
         class T2,
         class T3,
         class T4,
         class T5,
         class T6,
         class T7>
struct variant_alternative<I, variant<T0, T1, T2, T3, T4, T5, T6, T7> > {
	typedef typename variant<T0, T1, T2, T3, T4, T5, T6, T7>::template type_at<
	   I>::type type;
};

template<size_t I, class T>
struct variant_alternative<I, const T> : public variant_alternative<I, T> {};

template<size_t I, class T>
struct variant_alternative<I, volatile T> : public variant_alternative<I, T> {};

template<size_t I, class T>
struct variant_alternative<I, const volatile T>
  : public variant_alternative<I, T> {};

} /* namespace sstd */

#endif /* STATIC_VARIANT_H_ */
