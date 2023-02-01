#ifndef VECTOR_H
# define VECTOR_H

# include <iostream>
# include <memory>
# include <algorithm>
# include <cstddef>
# include <stdexcept>
# include <sstream>
# include "iterator_traits.hpp"
# include "iterator.hpp"
# include "utilities.hpp"

namespace ft
{
	template <class T, class Allocator = std::allocator<T> > class vector {

	public:
		typedef T														value_type;
		typedef std::size_t												size_type;
		typedef Allocator												allocator_type;
		typedef std::ptrdiff_t											difference_type;
		typedef value_type&												reference;
		typedef const value_type&										const_reference;
		typedef typename Allocator::pointer								pointer;
		typedef typename Allocator::const_pointer						const_pointer;		
		typedef ft::move_iterator<value_type>							iterator;
		typedef ft::move_iterator<const value_type>						const_iterator;
		typedef ft::reverse_iterator<iterator>							reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>					const_reverse_iterator;

	public:
		/****************************************************************************\
		**								Member functions							**
		\****************************************************************************/

		vector() : _size(), _capacity(), _v(NULL) {}

		explicit vector( const Allocator& alloc ) : _size(), _capacity(), _alloc(alloc), _v(NULL) {}

		explicit vector( size_type count, const value_type& value = value_type(), const Allocator& alloc = Allocator() ) : _alloc(alloc), _v(NULL)
		{
			_size = _capacity = count;
			_v = _alloc.allocate(capacity());
			for (iterator p = _v; p < _v + size(); ++p)
				_alloc.construct(&*p, value);
		}

		template <class InputIt>
		vector( InputIt first, InputIt last, const Allocator& alloc = Allocator(),
				typename ft::enable_if<!ft::is_integral<InputIt>::value, InputIt>::value* = 0 ) : _alloc(alloc), _v(NULL)
		{
			size_type	count(0);

			for (InputIt p = first; p != last; ++p)
				count++;
			_size = _capacity = count;
			_v = _alloc.allocate(capacity());
			assign(first, last);
		}

		vector( const vector& other ) : _size(other._size), _capacity(other._capacity), _alloc(other._alloc)
		{
			const_iterator	first = other.begin();
			const_iterator	last = other.end();
			ptrdiff_t		dist = last - first;

			_v = _alloc.allocate(_capacity);
			for (iterator p = _v; p < _v + dist && first != last; ++p, ++first)
				_alloc.construct(&*p, *first);
		};

		~vector()
		{
			clear();
			_alloc.deallocate(_v, capacity());
		}

		vector&	operator= ( const vector& rhs )
		{
			if (this == &rhs)
				return (*this);
			clear();
			_alloc.deallocate(_v, capacity());

			const_iterator	first = rhs.begin();
			const_iterator	last = rhs.end();
			ptrdiff_t		dist = last - first;

			_size = rhs._size;
			_capacity = rhs._capacity;
			_alloc = rhs._alloc;
			_v = _alloc.allocate(_capacity);
			for (iterator p = _v; p < _v + dist && first < last; ++p, ++first)
				_alloc.construct(&*p, *first);
			return (*this);
		}

		allocator_type	get_allocator() const { return _alloc; }

		/****************************************************************************\
		**								Element access								**
		\****************************************************************************/

		reference	at( size_type pos ) {
			if (pos >= size())
				throw (_out_of_range(pos));
			else
				return (_v[pos]);
		};
		
		const_reference	at( size_type pos ) const {
			if (pos >= size())
				throw (_out_of_range(pos));
			else
				return (_v[pos]);
		};

		reference		operator[]( size_type pos ) { return _v[pos]; };

		const_reference	operator[]( size_type pos ) const { return _v[pos]; }

		reference front() { return (*begin()); }

		const_reference front() const { return (*begin()); }

		reference back() { return *(end() - 1); };

		const_reference back() const { return *(end() - 1); };

		value_type*	data() { return (size() ? _v : NULL); };

		const value_type*	data() const { return (size() ? _v : NULL); };

		/****************************************************************************\
		**									Iterators								**
		\****************************************************************************/

		iterator				begin()	{ return (iterator(_v)); }
		const_iterator			begin() const { return (const_iterator(_v)); }
		iterator				end() { return (iterator(_v + size())); }
		const_iterator			end() const { return (const_iterator(_v + size())); }

		reverse_iterator		rbegin() { return (reverse_iterator(&_v[_size])); }
		const_reverse_iterator	rbegin() const { return (const_reverse_iterator(&_v[_size])); }
		reverse_iterator		rend() { return (reverse_iterator(begin())); }
		const_reverse_iterator	rend() const { return (const_reverse_iterator(begin())); }

		/****************************************************************************\
		**									Capacity								**
		\****************************************************************************/

		bool empty() const { return (!size()); }

		size_type	size() const { return (_size); }

		size_type	max_size() const { return std::min<size_type>(_alloc.max_size(),
								std::numeric_limits<difference_type>::max()); }
		
		size_type	capacity() const { return _capacity; }

		/****************************************************************************\
		**									Modifiers								**
		\****************************************************************************/

		void	assign( size_type count, const T& value ) {
			clear();
			if (capacity() < count)
				reserve(count);
			for (iterator p = _v; p < _v + count; ++p)
				_alloc.construct(&*p, value);
			_size = count;
		}

		template<class InputIt>
		void	assign( InputIt first, InputIt last, typename ft::enable_if<!ft::is_integral<InputIt>::value, InputIt>::value* = 0 )
		{
			size_type	pos(0);

			for (InputIt p = first; p != last; ++p)
				pos++;
			clear();
			if (pos > capacity())
			{
				_alloc.deallocate(_v, _capacity);
				_capacity = pos;
				_v = _alloc.allocate(pos);
			}
			for (; first != last; first++)
				_alloc.construct(&_v[_size++], *first);
		}

		void	reserve(size_type n)
		{
			if (n > max_size())
				throw std::length_error("length_error");
			if (n != 0 && n > _capacity)
			{
				pointer	tmp = _alloc.allocate(n);

				_capacity = n;
				for (size_t i = 0; i < _size; i++)
					_alloc.construct(&tmp[i], _v[i]);
				for (size_t i = 0; i < _size; i++)
					_alloc.destroy(&_v[i]);
				_alloc.deallocate(_v, _capacity);
				_v = tmp;
			}
		}

		void	clear() {
			for (iterator p = begin(); p != end(); ++p)
				_alloc.destroy(&*p);
			_size = 0;
		}

		// https://stackoverflow.com/questions/40767476/how-does-rhs-work

		iterator	insert(iterator position, const value_type &val)
		{
    		difference_type	pos = position - begin();

			insert(position, 1, val);
			return (iterator(begin() + pos));
		};

		void	insert(iterator pos, size_type count, const T& value)
		{
			difference_type dist = pos - begin();

			if (capacity() == 0)
				_extend();
			if (size() + count >= capacity())
				reserve(size() + count);
			vector temp(begin() + dist, end());

			for (size_t i = 0; i < temp.size(); i++)
				pop_back();
			while (count > 0)
			{
				push_back(value);
				count--;
			}
			for (iterator it = temp.begin(); it != temp.end(); it++)
				push_back(*it);
		};

		template <class InputIt>
		iterator	insert(iterator pos, InputIt first, InputIt last, typename ft::enable_if<!ft::is_integral<InputIt>::value, InputIt>::value* = 0)
		{
			size_type	count(0);

			for (InputIt p = first; p != last; ++p)
				count++;
			difference_type		dist = pos - this->begin();

			if (capacity() == 0)
				_extend();
			if (size() + count >= capacity())
				reserve(size() + count);
			vector		temp(this->begin() + dist, this->end());

			for (size_t i = 0; i < temp.size(); i++)
				this->pop_back();
			while (first != last)
			{
				this->push_back(*first);
				first++;
			}
			for (iterator it = temp.begin(); it != temp.end(); it++)
				this->push_back(*it);
			return (begin() + dist);
		}

		iterator	erase(iterator pos)
		{
			if (pos >= begin() && pos < end())
			{
				_alloc.destroy(&*pos);
				std::copy(pos + 1, end(), pos);
				--_size;
				return (pos);
			}
			else
				throw (_out_of_range(pos - begin()));
		};

		iterator	erase(iterator first, iterator last)
		{
			if ((first >= begin() && first <= end()) && (last >= begin() && last <= end())
				&& first <= last)
			{
				difference_type	dist = last - first;

				for (iterator it = first; it < last; ++it)
					_alloc.destroy(&*it);
				std::copy(last, end(), first);
				_size -= dist;
				return (first);
			}
			else
				throw (_out_of_range(last - begin()));
		}

		void	push_back(const value_type& value)
		{
			if (capacity() == 0 || size() == capacity())
				_extend();
			_alloc.construct(&*(_v + size()), value);
			++_size;
		}

		void	pop_back()
		{
			if (size())
			{
				pointer	last = &_v[size() - 1];

				_alloc.destroy(&*last);
				--_size;
			}
		}

		void push_front( const value_type& value )
		{
			insert(begin() + 1, value);
		}
		
		void pop_front()
		{
			if (size())
			{
				pointer	first = _v;

				_alloc.destroy(&*first);
				if (size() - 1 == capacity() / 2)
				{
					pointer	temp = _alloc.allocate(capacity() / 2);
					iterator p = temp;

					std::copy(begin() + 1, end(), p);
					_alloc.deallocate(_v, capacity());
					_capacity /= 2;
					_v = temp;
				}
				else
					std::copy(begin() + 1, end(), _v);
				--_size;
			}
		}

		void	resize(size_type count, value_type val = value_type())
		{
			if (count <= max_size())
			{
				if (count > capacity() && count < capacity() * 2)
				{
					pointer	temp = _alloc.allocate(capacity() * 2);
					iterator p = temp; 

					std::copy(begin(), end(), p);
					for(iterator q = p + size(); q < p + count; ++q)
						_alloc.construct(&*q, val);
					_v = temp;
					_size = count;
					_capacity *= 2;
				}
				else if (count > capacity() * 2)
				{
					pointer	temp = _alloc.allocate(count);
					iterator p = temp;

					std::copy(begin(), end(), p);
					for(iterator q = p + size(); q < p + count; ++q)
						_alloc.construct(&*q, val);
					_alloc.deallocate(_v, capacity());
					_v = temp;
					_size = _capacity = count;
				}
				else if (count > size())
				{
					for (iterator p = end(); p < _v + count; ++p)
						_alloc.construct(&*p, val);
					_size = count;
				}
				else if (count < size())
				{
					for (iterator p = _v + count; p < _v + size(); ++p)
						_alloc.destroy(&*p);
					_size = count;
				}
			}
			else
				throw (_out_of_range(count));
		}

		void	swap(vector& other)
		{
			std::swap(_v, other._v);
			std::swap(_size, other._size);
			std::swap(_capacity, other._capacity);
			std::swap(_alloc, other._alloc);
		}

	private:

		void	_extend()
		{
			if (capacity() == 0)
			{
				_v = _alloc.allocate(1);
				++_capacity;
			}
			else
			{
				pointer temp = _alloc.allocate(capacity() * 2);
				for (size_t i = 0; i < size(); i++)
				{
					_alloc.construct(temp + i, *(_v + i));
					_alloc.destroy(_v + i);
				}
				_alloc.deallocate(_v, capacity());
				_v = temp;
				_capacity *= 2;
			}
		};

		std::out_of_range	_out_of_range(size_type pos) const
		{
			std::stringstream ss;

			ss << "vector::_M_range_check: __n (which is " << pos << ")" <<	\
			" >= this->size() (which is " << size() << ")";
			return (std::out_of_range(ss.str()));
		};

		size_type		_size;
		size_type		_capacity;
		allocator_type	_alloc;
		pointer			_v;
	};

	template<class T, class Alloc>
	bool	operator== (const ft::vector<T, Alloc> &lhs, const ft::vector<T, Alloc> &rhs)
	{
		if (lhs.size() != rhs.size())
			return (false);
		typename vector<T, Alloc>::const_iterator lstart = lhs.begin();
		typename vector<T, Alloc>::const_iterator lend = lhs.end();
		typename vector<T, Alloc>::const_iterator rstart = rhs.begin();
		typename vector<T, Alloc>::const_iterator rend = rhs.end();
		for (; lstart != lend && rstart != rend; ++lstart, ++rstart)
			if (*lstart != *rstart)
				return (false);
		return (true);
	}

	template<class T, class Alloc>
	bool	operator!= (const ft::vector<T, Alloc> &lhs, const ft::vector<T, Alloc> &rhs)
	{
		return (!(lhs == rhs));
	}

	template<class T, class Alloc>
	bool	operator< (const ft::vector<T, Alloc> &lhs, const ft::vector<T, Alloc> &rhs)
	{
		return (ft::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end()));
	}

	template<class T, class Alloc>
	bool	operator> (const ft::vector<T, Alloc> &lhs, const ft::vector<T, Alloc> &rhs)
	{
		return (rhs < lhs);
	}

	template<class T, class Alloc>
	bool	operator<= (const ft::vector<T, Alloc> &lhs, const ft::vector<T, Alloc> &rhs)
	{
		return (!(rhs < lhs));
	}

	template<class T, class Alloc>
	bool	operator>= (const ft::vector<T, Alloc> &lhs, const ft::vector<T, Alloc> &rhs)
	{
		return (!(rhs > lhs));
	}
}

#endif