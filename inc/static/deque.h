/** @file    deque.h
 *  @brief   This header is a part of the containers library.
 *  @ingroup sequence_container
 */

#ifndef STATIC_DEQUE_H_
#define STATIC_DEQUE_H_

#include <stddef.h>

#include "algorithm.h"
#include "iterator.h"
#include "memory.h"
#include "type_traits.h"

namespace sstd {

template<class T, size_t N = 0>
class deque;

/** @brief  Bidirectional iterator for deque class.
 *  @tparam T  Deque type that the iterator belongs to.
 *  @tparam Const  Determines whether the iterator is a const iterator or not.
 */
template<class T, bool Const = false>
class deque_iterator {
	friend T;

	typedef typename conditional<Const, const T, T>::type deque_type;

  public:
	typedef bidirectional_iterator_tag iterator_category;
	typedef typename conditional<Const,
	                             const typename T::value_type,
	                             typename T::value_type>::type value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef typename T::size_type size_type;
	typedef typename T::difference_type difference_type;

	/** @brief Default construct. */
	deque_iterator() : deque_(0), pos_(0) {}

	/** @brief Construct from a deque and a position into it.
	 *  @param deq  The deque to iterator over.
	 *  @param pos  The position currently pointed to by the iterator.
	 */
	deque_iterator(deque_type* deq, difference_type pos) :
	  deque_(deq),
	  pos_(pos) {}

	/** @brief Implicit conversion from iterator to const iterator. */
	operator deque_iterator<T, true>() {
		return deque_iterator<T, true>(deque_, pos_);
	}

	/** @brief Returns a reference to the element at the current position. */
	reference operator*() { return (*deque_)[pos_]; }
	const_reference operator*() const { return (*deque_)[pos_]; }

	/** @brief Returns a pointer to the element at the current position. */
	pointer operator->() { return deque_->data_ + deque_->index_at(pos_); }
	const_reference operator->() const {
		return deque_->data_ + deque_->index_at(pos_);
	}

	/** @brief Pre-increment operator. */
	deque_iterator& operator++() {
		++pos_;
		return *this;
	}

	/** @brief Post-increment operator. */
	deque_iterator operator++(int) {
		deque_iterator it(*this);
		++pos_;
		return it;
	}

	/** @brief Pre-decrement operator. */
	deque_iterator& operator--() {
		--pos_;
		return *this;
	}

	/** @brief Post-decrement operator. */
	deque_iterator operator--(int) {
		deque_iterator it(*this);
		--pos_;
		return it;
	}

	/** @brief Returns a new iterator moved forward by n positions.
	 *  @param n  Number of positions to advance the iterator by.
	 */
	deque_iterator operator+(difference_type n) const {
		return deque_iterator(deque_, pos_ + n);
	}

	/** @brief Moves the iterator forward n positions.
	 *  @param n  Number of positions to advance the iterator by.
	 */
	deque_iterator& operator+=(difference_type n) {
		pos_ += n;
		return *this;
	}

	/** @brief Returns a new iterator moved back by n positions.
	 *  @param n  Number of positions to reverse the iterator by.
	 */
	deque_iterator operator-(difference_type n) const {
		return deque_iterator(deque_, pos_ - n);
	}

	/** @brief Moves the iterator back n positions.
	 *  @param n  Number of positions to reverse the iterator by.
	 */
	deque_iterator& operator-=(difference_type n) {
		pos_ -= n;
		return *this;
	}

	/** @brief Compare two iterators for equality.
	 *  @param rhs  The iterator being compared to this one.
	 */
	bool operator==(const deque_iterator& rhs) const {
		return deque_ == rhs.deque_ && pos_ == rhs.pos_;
	}

	/** @brief Compare two iterators for inequality.
	 *  @param rhs  The iterator being compared to this one.
	 */
	bool operator!=(const deque_iterator& rhs) const {
		return deque_ != rhs.deque_ || pos_ != rhs.pos_;
	}

  private:
	deque_type* deque_;
	difference_type pos_;
};

/** @brief Specialization for deque iterators. */
template<class T, bool C>
struct iterator_traits<deque_iterator<T, C> > {
	typedef typename deque_iterator<T, C>::iterator_category iterator_category;
	typedef typename deque_iterator<T, C>::value_type value_type;
	typedef typename deque_iterator<T, C>::pointer pointer;
	typedef typename deque_iterator<T, C>::reference reference;
	typedef typename deque_iterator<T, C>::difference_type difference_type;
};

/** @brief  Sequence container that emulates dynamic size arrays.
 *  @tparam T  Type stored by the container.
 *
 * A container that emulates dynamic size arrays with fixed capacity. This is a
 * base class which does not itself bring along any storage. As such, it cannot
 * be instantiated directly, and instead should be used through a @ref deque
 * instance with the size specified. You can use references to this class to
 * pass size agnostic deques around.
 */
template<class T>
class deque<T> {
  public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef deque_iterator<deque> iterator;
	typedef deque_iterator<deque, true> const_iterator;
	typedef sstd::reverse_iterator<iterator> reverse_iterator;
	typedef sstd::reverse_iterator<const_iterator> const_reverse_iterator;

	/** @brief Copy assignment operator.
	 *  @param rhs  The deque being assigned into this instance.
	 */
	deque& operator=(const deque& rhs) {
		assign(rhs.begin(), rhs.end());
		return *this;
	}

	/** @brief  Assign from a compatible deque.
	 *  @tparam T2  Compatible type to assign from.
	 *  @param  rhs  The deque being assigned into this instance.
	 */
	template<class T2>
	deque& operator=(const deque<T2>& rhs) {
		assign(rhs.begin(), rhs.end());
		return *this;
	}

	/** @brief Random access operator.
	 *  @param pos  Position of the element within the container.
	 */
	reference operator[](size_type pos) { return data_[index_at(pos)]; }
	const_reference operator[](size_type pos) const {
		return data_[index_at(pos)];
	}

	/** @brief Replaces the contents with count copies of value.
	 *  @param count  Number of elements to assign.
	 *  @param val  Value to copy into the container.
	 */
	void assign(size_type count, const_reference val) {
		resize(count);
		fill_n(begin(), size_, val);
	}

	/** @brief  Replaces the contents with range [first, last].
	 *  @tparam InputIt  The iterator type of the range being copied.
	 *  @param  first  The begin of the range to be copied.
	 *  @param  last  The end of the range to be copied.
	 */
	template<class InputIt>
	void assign(InputIt first, InputIt last) {
		typedef typename is_integral<InputIt>::type integral;
		assign_range_dispatch(first, last, integral());
	}

	/** @brief Access specified element with bounds checking.
	 *  @param pos  Position of the element within the container.
	 */
	reference at(size_type pos) { return data_[index_at(pos % capacity_)]; }
	const_reference at(size_type pos) const {
		return data_[index_at(pos % capacity_)];
	}

	/** @brief Access the first element. */
	reference front() { return data_[head_]; }
	const_reference front() const { return data_[head_]; }

	/** @brief Access the last element. */
	reference back() { return data_[tail_]; }
	const_reference back() const { return data_[tail_]; }

	/** @brief Returns an iterator to the first element. */
	iterator begin() { return iterator(this, 0); }
	const_iterator begin() const { return const_iterator(this, 0); }
	const_iterator cbegin() const { return const_iterator(this, 0); }

	/** @brief Returns an iterator to one past the last element. */
	iterator end() { return iterator(this, size_); }
	const_iterator end() const { return const_iterator(this, size_); }
	const_iterator cend() const { return const_iterator(this, size_); }

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
	bool empty() const { return size_ == 0; }

	/** @brief Returns the number of elements in the container. */
	size_type size() const { return size_; }

	/** @brief Returns the maximum possible number of elements. */
	size_type max_size() const { return capacity_; }

	/** @brief Removes all elements from the container. */
	void clear() { resize(0); }

	/** @brief Inserts val before pos.
	 *  @param pos  The iterator to insert the value ahead of.
	 *  @param val  The value to insert into the container.
	 */
	iterator insert(const_iterator pos, const_reference val) {
		// TODO: can we optimize rotation based on the distance from pos to
		// begin vs pos to end?

		iterator it(this, pos.pos_);

		if (size_ < capacity_) {
			resize(size_ + 1);
			const difference_type d = distance(cbegin(), pos);
			rotate(rbegin(), rbegin() + 1, reverse_iterator(begin() + d));
			*it = val;
		}

		return it;
	}

	/** @brief Inserts count copies of val before pos.
	 *  @param pos  The iterator to insert the values ahead of.
	 *  @param count  The number of copies to insert.
	 *  @param val  The value to insert into the container.
	 */
	iterator insert(const_iterator pos, size_type count, const_reference val) {
		// TODO: can we optimize rotation based on the distance from pos to
		// begin vs pos to end? Is count a factor?

		iterator it(this, pos.pos_);

		if (size_ < capacity_) {
			count = min(count, capacity_ - size_);
			resize(size_ + count);
			const difference_type d = distance(cbegin(), pos);
			rotate(rbegin(), rbegin() + count, reverse_iterator(begin() + d));
			fill_n(it, count, val);
		}

		return it;
	}

	/** @brief  Inserts copies of range before pos.
	 *  @tparam InputIt  The type of the iterator fo the range.
	 *  @param  pos  The iterator to insert the values ahead of.
	 *  @param  first  The begin of the range to copy.
	 *  @param  last  The end of the range to copy.
	 */
	template<class InputIt>
	iterator insert(const_iterator pos, InputIt first, InputIt last) {
		// TODO: can we optimize rotation based on the distance from pos to
		// begin vs pos to end? Is distance(first, last) a factor?
		typedef typename is_integral<InputIt>::type integral;
		return insert_range_dispatch(pos, first, last, integral());
	}

	/** @brief Removes specified element from the container.
	 *  @param pos  An iterator to the element being erased.
	 */
	iterator erase(const_iterator pos) {
		iterator it(this, pos.pos_);
		rotate(it, it + 1, end());
		resize(size_ - 1);
		return it;
	}

	/** @brief Removes specified elements from the container.
	 *  @param first  The begin of the range in the container to be erased.
	 *  @param last  The end of the range in the container to be erased.
	 */
	iterator erase(const_iterator first, const_iterator last) {
		iterator start(this, first.pos_);
		iterator finish(this, last.pos_);
		rotate(start, finish, end());
		resize(size_ - distance(start, finish));
		return start;
	}

	/** @brief Appends the given element value to the end of the container.
	 *  @param value  Reference to the value being copied.
	 */
	void push_back(const_reference value) {
		if (size_ < capacity_) {
			++size_;
			tail_ = increment_index(tail_);
			new (static_cast<void*>(data_ + tail_)) value_type(value);
		}
	}

	/** @brief Removes the last element of the container. */
	void pop_back() {
		if (size_) {
			destroy_at(data_ + tail_);
			tail_ = decrement_index(tail_);
			--size_;
		}
	}

	/** @brief Prepends the given element value to the front of the container.
	 *  @param value  Reference to the value being copied.
	 */
	void push_front(const_reference value) {
		if (size_ < capacity_) {
			++size_;
			head_ = decrement_index(head_);
			new (static_cast<void*>(data_ + head_)) value_type(value);
		}
	}

	/** @brief Removes the first element of the container. */
	void pop_front() {
		if (size_) {
			destroy_at(data_ + head_);
			head_ = increment_index(head_);
			--size_;
		}
	}

	/** @brief Resizes the container to contain count elements.
	 *  @param count  Desired size of the container.
	 *
	 * If the container is being made smaller, count minus size elements will be
	 * destroyed from the end. If the container gets bigger, size minus count
	 * values will be default initialized at the end.
	 */
	void resize(size_type count) {
		while (count < size_) { pop_back(); }
		while (count > size_) { push_back(value_type()); }
	}

	/** @brief Resizes the container to contain count elements.
	 *  @param count  Desired size of the container.
	 *  @param value  Value to use if expanding the container.
	 *
	 * If the container is being made smaller, count minus size elements will be
	 * destroyed from the end. If the container gets bigger, size minus count
	 * values will be copy initialized at the end.
	 */
	void resize(size_type count, const value_type& value) {
		while (count < size_) { pop_back(); }
		while (count > size_) { push_back(value); }
	}

  protected:
	/** @brief Construct with data, size, and capacity from child container.
	 *  @param data  Pointer to the storage for the container.
	 *  @param capacity  Capacity of the containers underlying storage.
	 */
	deque(T* data, size_type capacity) :
	  size_(0),
	  head_(0),
	  tail_(capacity - 1),
	  data_(data),
	  capacity_(capacity) {}

	~deque() {}

	size_type size_;
	size_type head_;
	size_type tail_;

  private:
	size_type index_at(size_type pos) const {
		size_type rollover = capacity_ - head_;
		return pos < rollover ? head_ + pos : pos - rollover;
	}

	size_type increment_index(size_type index) {
		++index;
		return index < capacity_ ? index : 0;
	}

	size_type decrement_index(size_type index) {
		return index ? index - 1 : capacity_ - 1;
	}

	template<class Int>
	void assign_range_dispatch(Int count, Int val, true_type) {
		assign(size_type(count), const_reference(val));
	}

	template<class InputIt>
	void assign_range_dispatch(InputIt first, InputIt last, false_type) {
		resize(min(distance(first, last), difference_type(capacity_)));
		copy_n(first, size_, begin());
	}

	template<class Int>
	iterator insert_range_dispatch(const_iterator pos,
	                               Int first,
	                               Int last,
	                               true_type) {
		return insert(pos, size_type(first), value_type(last));
	}

	template<class InputIt>
	iterator insert_range_dispatch(const_iterator pos,
	                               InputIt first,
	                               InputIt last,
	                               false_type) {
		iterator it(this, pos.pos_);

		if (size_ < capacity_) {
			difference_type count =
			   min(distance(first, last), difference_type(capacity_ - size_));
			resize(size_ + count);
			const difference_type d = distance(cbegin(), pos);
			rotate(rbegin(), rbegin() + count, reverse_iterator(begin() + d));
			copy_n(first, count, it);
		}

		return it;
	}

	const pointer data_;
	const size_type capacity_;
};

/** @brief  Fixed capacity storage for a deque<T> sequence container.
 *  @tparam T  Type stored by the container.
 *  @tparam N  Number of elements in the container.
 *
 * This class is intended to be used wherever a concrete instance of a deque is
 * required, and contains the necessary contiguous storage for the deque<T>
 * base to allocate from. Because the storage is internal, this type does not
 * use any heap allocations and as a consequence is expensive to pass by copy.
 */
template<class T, size_t N>
class deque /** @cond */ : public deque<T> /** @endcond */ {
	typedef deque<T> base;

  public:
	typedef typename base::pointer pointer;
	typedef typename base::const_pointer const_pointer;
	typedef typename base::value_type value_type;
	typedef typename base::reference reference;
	typedef typename base::const_reference const_reference;
	typedef typename base::size_type size_type;
	typedef typename base::difference_type difference_type;
	typedef typename base::iterator iterator;
	typedef typename base::const_iterator const_iterator;
	typedef typename base::reverse_iterator reverse_iterator;
	typedef typename base::const_reverse_iterator const_reverse_iterator;

	/** @brief Default construct. */
	deque() : base(reinterpret_cast<T*>(&buffer_), N) {}

	/** @brief Copy construct.
	 *  @param other  Source deque being copied.
	 */
	deque(const deque& other) : base(reinterpret_cast<T*>(&buffer_), N) {
		base::size_ = other.size_;
		base::head_ = other.head_;
		base::tail_ = other.tail_;
		uninitialized_copy_n(other.begin(), base::size_, base::begin());
	}

	/** @brief  Construct from a compatible deque.
	 *  @tparam T2  Compatible type to copy from.
	 *  @param  other  Source deque being copied.
	 */
	template<class T2>
	deque(const deque<T2>& other) : base(reinterpret_cast<T*>(&buffer_), N) {
		base::size_ = min(other.size(), N);
		if (base::size_) { base::tail_ = base::size_ - 1; }
		uninitialized_copy_n(other.begin(), base::size_, base::begin());
	}

	/** @brief Construct the deque with default initialized elements.
	 *  @param count  The number of elements to initialize.
	 */
	explicit deque(size_type count) : base(reinterpret_cast<T*>(&buffer_), N) {
		base::size_ = min(count, N);
		if (base::size_) { base::tail_ = base::size_ - 1; }
		uninitialized_value_construct_n(base::begin(), base::size_);
	}

	/** @brief Construct the deque with copy initialized elements.
	 *  @param count  The number of elements to initialize.
	 *  @param val  The value to initialize the elements with.
	 */
	deque(size_type count, const_reference val) :
	  base(reinterpret_cast<T*>(&buffer_), N) {
		base::size_ = min(count, N);
		if (base::size_) { base::tail_ = base::size_ - 1; }
		uninitialized_fill_n(base::begin(), base::size_, val);
	}

	/** @brief  Construct the deque with values from range [first, last].
	 *  @tparam InputIt  The type of Iterator for the range.
	 *  @param  first  The beginning of the range being copied.
	 *  @param  last  The end of the range being copied.
	 */
	template<class InputIt>
	deque(InputIt first, InputIt last) :
	  base(reinterpret_cast<T*>(&buffer_), N) {
		typedef typename is_integral<InputIt>::type integral;
		construct_range_dispatch(first, last, integral());
	}

	~deque() { destroy(base::begin(), base::end()); }

	/** @brief Copy assignment operator.
	 *  @param rhs  The deque being assigned into this instance.
	 */
	deque& operator=(const deque& rhs) {
		base::assign(rhs.begin(), rhs.end());
		return *this;
	}

	/** @brief  Assign from a compatible deque.
	 *  @tparam T2  Compatible type to assign from.
	 *  @param  rhs  The deque being assigned into this instance.
	 */
	template<class T2>
	deque& operator=(const deque<T2>& rhs) {
		base::assign(rhs.begin(), rhs.end());
		return *this;
	}

  private:
	template<class Int>
	void construct_range_dispatch(Int count, Int val, true_type) {
		base::size_ = min(size_type(count), N);
		if (base::size_) { base::tail_ = base::size_ - 1; }
		uninitialized_fill_n(base::begin(), base::size_, value_type(val));
	}

	template<class InputIt>
	void construct_range_dispatch(InputIt first, InputIt last, false_type) {
		base::size_ = min(size_type(distance(first, last)), N);
		if (base::size_) { base::tail_ = base::size_ - 1; }
		uninitialized_copy_n(first, base::size_, base::begin());
	}

	typedef
	   typename aligned_storage<N * sizeof(T), alignment_of<T>::value>::type
	      aligned_buffer_type;

	aligned_buffer_type buffer_;
};

template<class T>
inline bool operator==(const deque<T>& lhs, const deque<T>& rhs) {
	return lhs.size() == rhs.size() &&
	       equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
}

template<class T>
inline bool operator!=(const deque<T>& lhs, const deque<T>& rhs) {
	return !(lhs == rhs);
}

template<class T>
inline bool operator<(const deque<T>& lhs, const deque<T>& rhs) {
	return lexicographical_compare(
	   lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

template<class T>
inline bool operator>(const deque<T>& lhs, const deque<T>& rhs) {
	return rhs < lhs;
}

template<class T>
inline bool operator<=(const deque<T>& lhs, const deque<T>& rhs) {
	return !(rhs < lhs);
}

template<class T>
inline bool operator>=(const deque<T>& lhs, const deque<T>& rhs) {
	return !(lhs < rhs);
}

} /* namespace sstd */

#endif /* STATIC_DEQUE_H_ */
