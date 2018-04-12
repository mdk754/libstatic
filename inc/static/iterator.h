/** @file    iterator.h
 *  @brief   This header is a part of the iterator library.
 *  @ingroup iterator
 */

#ifndef STATIC_ITERATOR_H_
#define STATIC_ITERATOR_H_

#include <stddef.h>

namespace sstd {

/** @brief An Iterator that can read from the pointed-to element. */
struct input_iterator_tag {};

/** @brief An Iterator that can write to the pointed-to element. */
struct output_iterator_tag {};

/** @brief An InputIterator that can be incremented to the next element. */
struct forward_iterator_tag : public input_iterator_tag {};

/** @brief A ForwardIterator that can be moved in both directions. */
struct bidirectional_iterator_tag : public forward_iterator_tag {};

/** @brief A BidirectionalIterator that can move to point to any element in
 *         constant time.
 */
struct random_access_iterator_tag : public bidirectional_iterator_tag {};

/** @brief  Type trait class that provides uniform interface to the properties
 *          of Iterator types.
 *  @tparam Iterator  The type of the Iterator whose traits are being queried.
 */
template<class Iterator>
struct iterator_traits {
	typedef typename Iterator::iterator_category iterator_category;
	typedef typename Iterator::value_type value_type;
	typedef typename Iterator::pointer pointer;
	typedef typename Iterator::reference reference;
	typedef typename Iterator::difference_type difference_type;
};

/** @brief Specialization for iterators that are a pointer. */
template<class T>
struct iterator_traits<T*> {
	typedef random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef T* pointer;
	typedef T& reference;
	typedef ptrdiff_t difference_type;
};

/** @brief Specialization for iterators that are a const pointer. */
template<class T>
struct iterator_traits<const T*> {
	typedef random_access_iterator_tag iterator_category;
	typedef T value_type;
	typedef const T* pointer;
	typedef const T& reference;
	typedef ptrdiff_t difference_type;
};

/** @brief  An Iterator adaptor that reverses the direction of an iterator.
 *  @tparam Iterator  The type of the Iterator to be reversed.
 */
template<class Iterator>
class reverse_iterator {
  public:
	typedef
	   typename iterator_traits<Iterator>::iterator_category iterator_category;
	typedef typename iterator_traits<Iterator>::value_type value_type;
	typedef typename iterator_traits<Iterator>::pointer pointer;
	typedef typename iterator_traits<Iterator>::reference reference;
	typedef typename iterator_traits<Iterator>::difference_type difference_type;

	/** @brief Default construct. */
	reverse_iterator() : it_() {}

	/** @brief Construct from an Iterator. */
	explicit reverse_iterator(Iterator it) : it_(it) {}

	/** @brief Returns the underlying base iterator. */
	Iterator base() const { return it_; }

	/** @brief Returns a reference to the element previous to current. */
	reference operator*() const {
		Iterator tmp(it_);
		return *--tmp;
	}

	/** @brief Returns a pointer to the element previous to current. */
	pointer operator->() const { return &(operator*()); }

	/** @brief Returns a reference to the element at the specified relative
	 *         location. */
	reference operator[](difference_type n) const { return it_[-n - 1]; }

	/** @brief Pre-increments the iterator, applied in reverse. */
	reverse_iterator& operator++() {
		--it_;
		return *this;
	}

	/** @brief Post-increments the iterator, applied in reverse. */
	reverse_iterator operator++(int) {
		reverse_iterator tmp(*this);
		--it_;
		return *tmp;
	}

	/** @brief Advances the iterator by n positions. */
	reverse_iterator& operator+=(difference_type n) { return it_ -= n; }

	/** @brief Returns an iterator which is advanced by n positions. */
	reverse_iterator operator+(difference_type n) const {
		return reverse_iterator(it_ - n);
	}

	/** @brief Pre-decrements the iterator, applied in reverse. */
	reverse_iterator& operator--() {
		++it_;
		return *this;
	}

	/** @brief Post-decrements the iterator, applied in reverse. */
	reverse_iterator operator--(int) {
		reverse_iterator tmp(*this);
		++it_;
		return *tmp;
	}

	/** @brief Advances the iterator by -n positions. */
	reverse_iterator& operator-=(difference_type n) { return it_ += n; }

	/** @brief Returns an iterator which is advanced by -n positions. */
	reverse_iterator operator-(difference_type n) const {
		return reverse_iterator(it_ + n);
	}

  protected:
	Iterator it_;
};

template<class Iterator1, class Iterator2>
bool operator==(const reverse_iterator<Iterator1>& lhs,
                const reverse_iterator<Iterator2>& rhs) {
	return lhs.base() == rhs.base();
}

template<class Iterator1, class Iterator2>
bool operator!=(const reverse_iterator<Iterator1>& lhs,
                const reverse_iterator<Iterator2>& rhs) {
	return !(lhs == rhs);
}

template<class Iterator1, class Iterator2>
bool operator<(const reverse_iterator<Iterator1>& lhs,
               const reverse_iterator<Iterator2>& rhs) {
	return lhs.base() < rhs.base();
}

template<class Iterator1, class Iterator2>
bool operator>(const reverse_iterator<Iterator1>& lhs,
               const reverse_iterator<Iterator2>& rhs) {
	return rhs < lhs;
}

template<class Iterator1, class Iterator2>
bool operator<=(const reverse_iterator<Iterator1>& lhs,
                const reverse_iterator<Iterator2>& rhs) {
	return !(rhs < lhs);
}

template<class Iterator1, class Iterator2>
bool operator>=(const reverse_iterator<Iterator1>& lhs,
                const reverse_iterator<Iterator2>& rhs) {
	return !(lhs < rhs);
}

/** @brief  Returns the number of hops from first to last.
 *  @tparam InputIt  The iterator type of the range.
 */
template<class InputIt>
inline typename iterator_traits<InputIt>::difference_type distance(
   InputIt first,
   InputIt last) {
	typename iterator_traits<InputIt>::difference_type size = 0;

	while (first != last) {
		++size;
		++first;
	}

	return size;
}

} /* namespace sstd */

#endif /* STATIC_ITERATOR_H_ */
