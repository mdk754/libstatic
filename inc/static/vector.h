/** @file    vector.h
 *  @brief   This header is a part of the containers library.
 *  @ingroup sequence_container
 */

#ifndef STATIC_VECTOR_H_
#define STATIC_VECTOR_H_

#include <stddef.h>

#include "algorithm.h"
#include "iterator.h"
#include "memory.h"
#include "type_traits.h"

namespace sstd {

template<class T, size_t N = 0>
class vector;

/** @brief  Sequence container that emulates dynamic size arrays.
 *  @tparam T  Type stored by the container.
 *
 * A container that emulates dynamic size arrays with fixed capacity. This is a
 * base class which does not itself bring along any storage. As such, it cannot
 * be instantiated directly, and instead should be used through a @ref vector
 * instance with the size specified. You can use references to this class to
 * pass size agnostic vectors around.
 */
template<class T>
class vector<T> {
  public:
	typedef T value_type;
	typedef value_type* pointer;
	typedef const value_type* const_pointer;
	typedef value_type& reference;
	typedef const value_type& const_reference;
	typedef size_t size_type;
	typedef ptrdiff_t difference_type;
	typedef pointer iterator;
	typedef const_pointer const_iterator;
	typedef sstd::reverse_iterator<iterator> reverse_iterator;
	typedef sstd::reverse_iterator<const_iterator> const_reverse_iterator;

	/** @brief Copy assignment operator.
	 *  @param rhs  The vector being assigned into this instance.
	 */
	vector& operator=(const vector& rhs) {
		assign(rhs.begin(), rhs.end());
		return *this;
	}

	/** @brief  Assign from a compatible vector.
	 *  @tparam T2  Compatible type to assign from.
	 *  @param  rhs  The vector being assigned into this instance.
	 */
	template<class T2>
	vector& operator=(const vector<T2>& rhs) {
		assign(rhs.begin(), rhs.end());
		return *this;
	}

	/** @brief Random access operator.
	 *  @param pos  Position of the element within the container.
	 */
	reference operator[](size_type pos) { return data_[pos]; }
	const_reference operator[](size_type pos) const { return data_[pos]; }

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
	reference at(size_type pos) { return data_[pos % capacity_]; }
	const_reference at(size_type pos) const { return data_[pos % capacity_]; }

	/** @brief Access the first element. */
	reference front() { return data_[0]; }
	const_reference front() const { return data_[0]; }

	/** @brief Access the last element. */
	reference back() { return data_[size_ - 1]; }
	const_reference back() const { return data_[size_ - 1]; }

	/** @brief Access the underlying array pointer. */
	pointer data() { return data_; }
	const_pointer data() const { return data_; }

	/** @brief Returns an iterator to the first element. */
	iterator begin() { return data_; }
	const_iterator begin() const { return data_; }
	const_iterator cbegin() const { return data_; }

	/** @brief Returns an iterator to one past the last element. */
	iterator end() { return data_ + size_; }
	const_iterator end() const { return data_ + size_; }
	const_iterator cend() const { return data_ + size_; }

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

	/** @brief Returns the number of elements that the container has currently
	 *         allocated space for. */
	size_type capacity() const { return capacity_; }

	/** @brief Removes all elements from the container. */
	void clear() { resize(0); }

	/** @brief Inserts val before pos.
	 *  @param pos  The iterator to insert the value ahead of.
	 *  @param val  The value to insert into the container.
	 */
	iterator insert(const_iterator pos, const_reference val) {
		if (size_ < capacity_) {
			resize(size_ + 1);
			const difference_type d = distance(cbegin(), pos);
			rotate(rbegin(), rbegin() + 1, reverse_iterator(begin() + d));
			*iterator(pos) = val;
		}

		return iterator(pos);
	}

	/** @brief Inserts count copies of val before pos.
	 *  @param pos  The iterator to insert the values ahead of.
	 *  @param count  The number of copies to insert.
	 *  @param val  The value to insert into the container.
	 */
	iterator insert(const_iterator pos, size_type count, const_reference val) {
		if (size_ < capacity_) {
			count = min(count, capacity_ - size_);
			resize(size_ + count);
			const difference_type d = distance(cbegin(), pos);
			rotate(rbegin(), rbegin() + count, reverse_iterator(begin() + d));
			fill_n(iterator(pos), count, val);
		}

		return iterator(pos);
	}

	/** @brief  Inserts copies of range before pos.
	 *  @tparam InputIt  The type of the iterator fo the range.
	 *  @param  pos  The iterator to insert the values ahead of.
	 *  @param  first  The begin of the range to copy.
	 *  @param  last  The end of the range to copy.
	 */
	template<class InputIt>
	iterator insert(const_iterator pos, InputIt first, InputIt last) {
		typedef typename is_integral<InputIt>::type integral;
		return insert_range_dispatch(pos, first, last, integral());
	}

	/** @brief Removes specified element from the container.
	 *  @param pos  An iterator to the element being erased.
	 */
	iterator erase(const_iterator pos) {
		iterator it = iterator(pos);
		rotate(it, it + 1, end());
		resize(size_ - 1);
		return it;
	}

	/** @brief Removes specified elements from the container.
	 *  @param first  The begin of the range in the container to be erased.
	 *  @param last  The end of the range in the container to be erased.
	 */
	iterator erase(const_iterator first, const_iterator last) {
		iterator start  = iterator(first);
		iterator finish = iterator(last);
		rotate(start, finish, end());
		resize(size_ - distance(start, finish));
		return start;
	}

	/** @brief Appends the given element value to the end of the container.
	 *  @param value  Reference to the value being copied.
	 */
	void push_back(const T& value) {
		if (size_ < capacity_) {
			uninitialized_fill_n(end(), 1, value);
			++size_;
		}
	}

	/** @brief Removes the last element of the container. */
	void pop_back() { resize(size_ - 1); }

	/** @brief Resizes the container to contain count elements.
	 *  @param count  Desired size of the container.
	 *
	 * If the container is being made smaller, count minus size elements will be
	 * destroyed from the end. If the container gets bigger, size minus count
	 * values will be default initialized at the end.
	 */
	void resize(size_type count) {
		if (count <= size_) {
			destroy(begin() + count, end());
			size_ = count;
		} else {
			count = min(count, capacity_);
			uninitialized_value_construct(end(), begin() + count);
			size_ = count;
		}
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
		if (count <= size()) {
			destroy(begin() + count, end());
			size_ = count;
		} else {
			count = min(count, max_size());
			uninitialized_fill(end(), begin() + count, value);
			size_ = count;
		}
	}

  protected:
	/** @brief Construct with data, size, and capacity from child container.
	 *  @param data  Pointer to the storage for the container.
	 *  @param size  Initial size of the container.
	 *  @param capacity  Capacity of the containers underlying storage.
	 */
	vector(T* data, size_type size, size_type capacity) :
	  size_(size),
	  data_(data),
	  capacity_(capacity) {}

	~vector() {}

	size_type size_;

  private:
	template<class Int>
	void assign_range_dispatch(Int count, Int val, true_type) {
		assign(size_type(count), const_reference(val));
	}

	template<class InputIt>
	void assign_range_dispatch(InputIt first, InputIt last, false_type) {
		resize(distance(first, last));
		copy_n(first, size_, begin());
	}

	template<class InputIt>
	iterator insert_range_dispatch(const_iterator pos,
	                               InputIt first,
	                               InputIt last,
	                               true_type) {
		return insert(pos, size_type(first), value_type(last));
	}

	template<class InputIt>
	iterator insert_range_dispatch(const_iterator pos,
	                               InputIt first,
	                               InputIt last,
	                               false_type) {
		if (size_ < capacity_) {
			difference_type count =
			   min(distance(first, last), difference_type(capacity_ - size_));
			resize(size_ + count);
			const difference_type d = distance(cbegin(), pos);
			rotate(rbegin(), rbegin() + count, reverse_iterator(begin() + d));
			copy_n(first, count, iterator(pos));
		}

		return iterator(pos);
	}

	const pointer data_;
	const size_type capacity_;
};

/** @brief  Fixed capacity storage for a vector<T> sequence container.
 *  @tparam T  Type stored by the container.
 *  @tparam N  Number of elements in the container.
 *
 * This class is intended to be used wherever a concrete instance of a vector is
 * required, and contains the necessary contiguous storage for the vector<T>
 * base to allocate from. Because the storage is internal, this type does not
 * use any heap allocations and as a consequence is expensive to pass by copy.
 */
template<class T, size_t N>
class vector /** @cond */ : public vector<T> /** @endcond */ {
	typedef vector<T> base;

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
	vector() : base(reinterpret_cast<T*>(&buffer_), 0, N) {}

	/** @brief Copy construct.
	 *  @param other  Source vector being copied.
	 */
	vector(const vector& other) :
	  base(reinterpret_cast<T*>(&buffer_), other.size(), N) {
		uninitialized_copy_n(other.begin(), base::size_, base::begin());
	}

	/** @brief  Construct from a compatible vector.
	 *  @tparam T2  Compatible type to copy from.
	 *  @param  other  Source vector being copied.
	 */
	template<class T2>
	vector(const vector<T2>& other) :
	  base(reinterpret_cast<T*>(&buffer_), min(other.size(), N), N) {
		uninitialized_copy_n(other.begin(), base::size_, base::begin());
	}

	/** @brief Construct the vector with default initialized elements.
	 *  @param count  The number of elements to initialize.
	 */
	explicit vector(size_type count) :
	  base(reinterpret_cast<T*>(&buffer_), min(count, N), N) {
		uninitialized_value_construct_n(base::begin(), base::size_);
	}

	/** @brief Construct the vector with copy initialized elements.
	 *  @param count  The number of elements to initialize.
	 *  @param val  The value to initialize the elements with.
	 */
	vector(size_type count, const_reference val) :
	  base(reinterpret_cast<T*>(&buffer_), min(count, N), N) {
		uninitialized_fill_n(base::begin(), base::size_, val);
	}

	/** @brief  Construct the vector with values from range [first, last].
	 *  @tparam InputIt  The type of Iterator for the range.
	 *  @param  first  The beginning of the range being copied.
	 *  @param  last  The end of the range being copied.
	 */
	template<class InputIt>
	vector(InputIt first, InputIt last) :
	  base(reinterpret_cast<T*>(&buffer_), 0, N) {
		typedef typename is_integral<InputIt>::type integral;
		construct_range_dispatch(first, last, integral());
	}

	~vector() { destroy(base::begin(), base::end()); }

	/** @brief Copy assignment operator.
	 *  @param rhs  The vector being assigned into this instance.
	 */
	vector& operator=(const vector& rhs) {
		base::assign(rhs.begin(), rhs.end());
		return *this;
	}

	/** @brief  Assign from a compatible vector.
	 *  @tparam T2  Compatible type to assign from.
	 *  @param  rhs  The vector being assigned into this instance.
	 */
	template<class T2>
	vector& operator=(const vector<T2>& rhs) {
		base::assign(rhs.begin(), rhs.end());
		return *this;
	}

  private:
	template<class Int>
	void construct_range_dispatch(Int count, Int val, true_type) {
		base::size_ = min(size_type(count), N);
		uninitialized_fill_n(base::begin(), base::size_, value_type(val));
	}

	template<class InputIt>
	void construct_range_dispatch(InputIt first, InputIt last, false_type) {
		base::size_ = min(size_type(distance(first, last)), N);
		uninitialized_copy_n(first, base::size_, base::begin());
	}

	typedef
	   typename aligned_storage<N * sizeof(T), alignment_of<T>::value>::type
	      aligned_buffer_type;

	aligned_buffer_type buffer_;
};

template<class T>
inline bool operator==(const vector<T>& lhs, const vector<T>& rhs) {
	return lhs.size() == rhs.size() &&
	       equal(lhs.cbegin(), lhs.cend(), rhs.cbegin());
}

template<class T>
inline bool operator!=(const vector<T>& lhs, const vector<T>& rhs) {
	return !(lhs == rhs);
}

template<class T>
inline bool operator<(const vector<T>& lhs, const vector<T>& rhs) {
	return lexicographical_compare(
	   lhs.cbegin(), lhs.cend(), rhs.cbegin(), rhs.cend());
}

template<class T>
inline bool operator>(const vector<T>& lhs, const vector<T>& rhs) {
	return rhs < lhs;
}

template<class T>
inline bool operator<=(const vector<T>& lhs, const vector<T>& rhs) {
	return !(rhs < lhs);
}

template<class T>
inline bool operator>=(const vector<T>& lhs, const vector<T>& rhs) {
	return !(lhs < rhs);
}

} /* namespace sstd */

#endif /* STATIC_VECTOR_H_ */
