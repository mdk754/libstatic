/** @file    optional.h
 *  @brief   This header is a part of the utilities library.
 *  @ingroup general_purpose
 */

#ifndef STATIC_OPTIONAL_H_
#define STATIC_OPTIONAL_H_

#include "memory.h"
#include "type_traits.h"

namespace sstd {

/** @brief Empty class used to inidicate optional with uninitialized state. */
struct nullopt_t {
	// TODO: If we had extended braced initializers and constexpr we could
	// declare a constexpr constructor taking an integral type, to allow for the
	// optional_value = {} syntax.
	explicit nullopt_t(int) {}
};

/** @brief Constant of type nullopt_t that is used to indicate optional type
 *         with uninitialized state.
 */
const nullopt_t nullopt(0);

/** @brief  Manages an optional contained value, ie. may or may not be present.
 *  @tparam T  The type of the value to manage.
 *
 * A common use case for optional is for the return value from a function which
 * may fail. Instead of passing a struct, pair, or requiring the output as a
 * function parameter, optional is a more readable means for expressing the
 * intent. Any instance of optional<T> at any given time will either contain a
 * value or no value. If it contains a value, the value is allocated as a part
 * of the optional without dynamic allocation.
 */
template<class T>
class optional {
  public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;

	/** @brief Default construct. */
	optional() : valid_(false) {}

	/** @brief Construct in uninitialized state. */
	optional(nullopt_t) : valid_(false) {}

	/** @brief Copy construct.
	 *  @param other  The otional type being copied.
	 */
	optional(const optional& other) : valid_(bool(other)) {
		if (valid_) {
			new (static_cast<void*>(&buffer_)) value_type(other.value());
		}
	}

	/** @brief  Construct from a compatible type.
	 *  @tparam U  Compatible type from another optional.
	 *  @param  other  The optional type being copied.
	 */
	template<class U>
	explicit optional(const optional<U>& other) : valid_(bool(other)) {
		if (valid_) {
			new (static_cast<void*>(&buffer_)) value_type(other.value());
		}
	}

	/** @brief Construct from a value type.
	 *  @param val  Reference to the value being copied.
	 */
	optional(const_reference val) : valid_(true) {
		new (static_cast<void*>(&buffer_)) value_type(val);
	}

	~optional() {
		if (valid_) { destroy_at(reinterpret_cast<pointer>(&buffer_)); }
	}

	/** @brief Reset back to uninitialized state. */
	optional& operator=(nullopt_t) {
		if (valid_) {
			destroy_at(reinterpret_cast<pointer>(&buffer_));
			valid_ = false;
		}

		return *this;
	}

	/** @brief Assign from another optional type.
	 *  @param other  The optional type being copied.
	 */
	optional& operator=(const optional& other) {
		if (this == &other) { return *this; }

		if (bool(other)) {
			if (valid_) {
				value() = other.value();
			} else {
				new (static_cast<void*>(&buffer_)) value_type(other.value());
				valid_ = true;
			}
		} else {
			if (valid_) {
				destroy_at(reinterpret_cast<pointer>(&buffer_));
				valid_ = false;
			}
		}

		return *this;
	}

	/** @brief  Assign from a compatible optional type.
	 *  @tparam U  Compatible type from another optional.
	 *  @param  other  The optional type being copied.
	 */
	template<class U>
	optional& operator=(const optional<U>& other) {
		if (bool(other)) {
			if (valid_) {
				value() = other.value();
			} else {
				new (static_cast<void*>(&buffer_)) value_type(other.value());
				valid_ = true;
			}
		} else {
			if (valid_) {
				destroy_at(reinterpret_cast<pointer>(&buffer_));
				valid_ = false;
			}
		}

		return *this;
	}

	/** @brief Assign from a value type.
	 *  @param val  Reference to the value being copied.
	 */
	optional& operator=(const_reference val) {
		if (valid_) {
			value() = val;
		} else {
			new (static_cast<void*>(&buffer_)) value_type(val);
			valid_ = true;
		}

		return *this;
	}

	/** @brief Returns a pointer to the contained value. */
	pointer operator->() { return reinterpret_cast<pointer>(&buffer_); }
	const_pointer operator->() const {
		return reinterpret_cast<const_pointer>(&buffer_);
	}

	/** @brief Returns a reference to the contained value. */
	reference operator*() { return *operator->(); }
	const_reference operator*() const { return *operator->(); }

	/** @brief Checks whether *this contains a value. */
	operator bool() const { return valid_; }

	/** @brief Checks whether *this contains a value. */
	bool has_value() const { return valid_; }

	/** @brief Returns a reference to the contained value. */
	reference value() { return *operator->(); }
	const_reference value() const { return *operator->(); }

	/** @brief  Returns a the contained value if valid, otherwise returns a
	 *          user-supplied default value.
	 *  @tparam U  Type of the supplied value, must be compatible with T.
	 *  @param  default_value  The value returned if *this is invalid.
	 */
	template<class U>
	value_type value_or(const U& default_value) const {
		return valid_ ? value() : default_value;
	}

	/** @brief Reset back to uninitialized state. */
	void reset() {
		if (valid_) {
			destroy_at(reinterpret_cast<pointer>(&buffer_));
			valid_ = false;
		}
	}

  private:
	typedef typename aligned_storage<sizeof(T), alignment_of<T>::value>::type
	   aligned_buffer_type;

	aligned_buffer_type buffer_;
	bool valid_;
};

template<class T, class U>
inline bool operator==(const optional<T>& lhs, const optional<U>& rhs) {
	if (bool(lhs) != bool(rhs)) { return false; }
	if (!bool(lhs) && !bool(rhs)) { return true; }

	return lhs.value() == rhs.value();
}

template<class T, class U>
inline bool operator!=(const optional<T>& lhs, const optional<U>& rhs) {
	return !(lhs == rhs);
}

template<class T, class U>
inline bool operator<(const optional<T>& lhs, const optional<U>& rhs) {
	if (!bool(rhs)) { return false; }
	if (!bool(lhs)) { return true; }

	return lhs.value() < rhs.value();
}

template<class T, class U>
inline bool operator>(const optional<T>& lhs, const optional<U>& rhs) {
	return !(lhs <= rhs);
}

template<class T, class U>
inline bool operator<=(const optional<T>& lhs, const optional<U>& rhs) {
	if (!bool(lhs)) { return true; }
	if (!bool(rhs)) { return false; }

	return lhs.value() <= rhs.value();
}

template<class T, class U>
inline bool operator>=(const optional<T>& lhs, const optional<U>& rhs) {
	return !(lhs < rhs);
}

template<class T>
inline bool operator==(const optional<T>& lhs, nullopt_t) {
	return !bool(lhs);
}

template<class T>
inline bool operator!=(const optional<T>& lhs, nullopt_t) {
	return bool(lhs);
}

template<class T>
inline bool operator<(const optional<T>&, nullopt_t) {
	return false;
}

template<class T>
inline bool operator>(const optional<T>& lhs, nullopt_t) {
	return bool(lhs);
}

template<class T>
inline bool operator<=(const optional<T>& lhs, nullopt_t) {
	return !bool(lhs);
}

template<class T>
inline bool operator>=(const optional<T>&, nullopt_t) {
	return true;
}

template<class T>
inline bool operator==(nullopt_t, const optional<T>& rhs) {
	return !bool(rhs);
}

template<class T>
inline bool operator!=(nullopt_t, const optional<T>& rhs) {
	return bool(rhs);
}

template<class T>
inline bool operator<(nullopt_t, const optional<T>& rhs) {
	return bool(rhs);
}

template<class T>
inline bool operator>(nullopt_t, const optional<T>&) {
	return false;
}

template<class T>
inline bool operator<=(nullopt_t, const optional<T>&) {
	return true;
}

template<class T>
inline bool operator>=(nullopt_t, const optional<T>& rhs) {
	return !bool(rhs);
}

template<class T, class U>
inline bool operator==(const optional<T>& lhs, const U& val) {
	return bool(lhs) ? lhs.value() == val : false;
}

template<class T, class U>
inline bool operator!=(const optional<T>& lhs, const U& val) {
	return !(lhs == val);
}

template<class T, class U>
inline bool operator<(const optional<T>& lhs, const U& val) {
	return bool(lhs) ? lhs.value() < val : true;
}

template<class T, class U>
inline bool operator>(const optional<T>& lhs, const U& val) {
	return bool(lhs) ? lhs.value() > val : false;
}

template<class T, class U>
inline bool operator<=(const optional<T>& lhs, const U& val) {
	return bool(lhs) ? lhs.value() <= val : true;
}

template<class T, class U>
inline bool operator>=(const optional<T>& lhs, const U& val) {
	return bool(lhs) ? lhs.value() >= val : false;
}

template<class T, class U>
inline bool operator==(const U& val, const optional<T>& rhs) {
	return bool(rhs) ? rhs.value() == val : false;
}

template<class T, class U>
inline bool operator!=(const U& val, const optional<T>& rhs) {
	return !(val == rhs);
}

template<class T, class U>
inline bool operator<(const U& val, const optional<T>& rhs) {
	return bool(rhs) ? val < rhs.value() : false;
}

template<class T, class U>
inline bool operator>(const U& val, const optional<T>& rhs) {
	return bool(rhs) ? val > rhs.value() : true;
}

template<class T, class U>
inline bool operator<=(const U& val, const optional<T>& rhs) {
	return bool(rhs) ? val <= rhs.value() : false;
}

template<class T, class U>
inline bool operator>=(const U& val, const optional<T>& rhs) {
	return bool(rhs) ? val >= rhs.value() : true;
}

} /* namespace sstd */

#endif /* STATIC_OPTIONAL_H_ */
