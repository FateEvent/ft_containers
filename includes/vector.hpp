#ifndef VECTOR_H
# define VECTOR_H

# include <iostream>
# include <memory>
# include <algorithm>
# include "iterator.hpp"
# include "ArrayException.hpp"

namespace ft
{
	template <class T, class Allocator = std::allocator<T> > class vector {

	public:
		typedef T										value_type;
		typedef std::size_t								size_type;
		typedef Allocator								allocator_type;
		typedef std::ptrdiff_t							difference_type;
		typedef value_type								&reference;
		typedef const value_type						&const_reference;
		typedef typename Allocator::pointer				pointer;
		typedef typename Allocator::const_pointer		const_pointer;		
		typedef ft::move_iterator<value_type>			iterator;
		typedef ft::const_iterator<value_type>			const_iterator;
		typedef ft::reverse_iterator<value_type>		reverse_iterator;
		typedef ft::const_reverse_iterator<value_type>	const_reverse_iterator;

	private:
		size_type	_size;
		size_type	_capacity;
		Allocator	_alloc;
		pointer		_v;

	public:
		vector() : _size(1), _capacity(1), _v(nullptr) {
			_v = _alloc.allocate(_capacity);
		}

		explicit vector( const Allocator& alloc ) : _size(1), _capacity(1), _alloc(alloc), _v(nullptr) {
			_v = _alloc.allocate(_capacity);
		}

		explicit vector( size_type count, const value_type& value = value_type(), const Allocator& alloc = Allocator() ) : _alloc(alloc), _v(nullptr)
		{
			_size = _capacity = count;
			_v = _alloc.allocate(_capacity);
			for (iterator p = _v; p < _v + _capacity; ++p)
				_alloc.construct(&*p, value);
		}

		template <class InputIt>
		vector( InputIt first, InputIt last, const Allocator& alloc = Allocator() ) : _alloc(alloc), _v(nullptr)
		{
			ptrdiff_t	dist = last - first;
			_size = _capacity = dist;
			_v = _alloc.allocate(dist);
			for (iterator p = _v; p < _v + dist && first < last; ++p, ++first)
				_alloc.construct(&*p, *first);
		};

		vector( const vector& other ) : _size(other._size), _capacity(other._capacity), _alloc(other._alloc)
		{
			const_iterator	first = other.begin();
			const_iterator	last = other.end();
			ptrdiff_t		dist = last - first;

			_v = _alloc.allocate(_capacity);
			for (iterator p = _v; p < _v + dist && first < last; ++p, ++first)
				_alloc.construct(&*p, *first);
		};

		~vector()
		{
			for (iterator p = _v; p < _v + size(); ++p)
				_alloc.destroy(&*p);
			_alloc.deallocate(_v, _capacity);
		}

		vector&	operator= ( const vector& rhs )
		{
			if (this == rhs)
				return (*this);
			for (iterator p = _v; p < _v + size(); ++p)
				_alloc.destroy(&*p);
			_alloc.deallocate(_v, _capacity);

			iterator	first = rhs.begin();
			iterator	last = rhs.end();
			ptrdiff_t	dist = last - first;

			_size = rhs._size;
			_capacity = rhs._capacity;
			_alloc = rhs._alloc;
			_v = _alloc.allocate(_capacity);
			for (iterator p = _v; p < _v + dist && first < last; ++p, ++first)
				_alloc.construct(&*p, *first);
			return (*this);
		}

		void	assign( size_type count, const T& value ) {
			for (iterator p = _v; p < _v + size(); ++p)
				_alloc.destroy(&*p);
			_alloc.deallocate(_v, _capacity);
			_size = _capacity = count;
			pointer	temp = _alloc.allocate(count);
			for (iterator p = temp; p < temp + count; ++p)
				_alloc.construct(&*p, value);
			_v = temp;
		}

		template <class InputIt >
		void	assign( InputIt first, InputIt last )
		{
			for (iterator p = _v; p < _v + size(); ++p)
				_alloc.destroy(&*p);
			_alloc.deallocate(_v, _capacity);
			ptrdiff_t	dist = last - first;
			_size = _capacity = dist;
			pointer	temp = _alloc.allocate(dist);
			for (iterator p = temp; p < temp + dist && first < last; ++p, ++first)
				_alloc.construct(&*p, *first);
			_v = temp;
		}

		allocator_type	get_allocator() const { return _alloc; }

		reference				at( size_type pos ) {
			if (pos >= size())
				throw (ArrayException("out_of_range"));
			return (&_v[pos]);
		};
		
		const_reference			at( size_type pos ) const {
			if (pos >= size())
				throw (ArrayException("out_of_range"));
			return (&_v[pos]);
		};

		reference		operator[]( size_type pos ) { return &_v[pos]; };
		const_reference	operator[]( size_type pos ) const { return &_v[pos]; }

		reference front() { return (&begin()); }

		const_reference front() const { return (&begin()); }

		reference back() { return (&end() - 1); };

		const_reference back() const { return (&end() - 1); };

		value_type*	data() { return (size() ? _v : NULL); };

		const value_type*	data() const { return (size() ? _v : NULL); };

		iterator				begin()	{ return iterator(&_v[0]); }
		const_iterator			begin() const	{ return const_iterator(&_v[0]); }
		iterator				end()	{ return iterator(&_v[_size]); }
		const_iterator			end() const	{ return const_iterator(&_v[_size]); }

		reverse_iterator		rbegin()	{ return reverse_iterator(&_v[_size - 1]); }
		const_reverse_iterator	rbegin() const	{ return const_reverse_iterator(&_v[_size - 1]); }
		reverse_iterator		rend()	{ return reverse_iterator(&_v[-1]); }
		const_reverse_iterator	rend() const	{ return const_reverse_iterator(&_v[-1]); }
		
		bool empty() const { return !size(); }

		size_type	size() { return _size; }

		size_type	max_size() { return std::min<size_type>(_alloc.max_size(),
								std::numeric_limits<difference_type>::max()); }

		void	reserve(size_type n)
		{
			if (n > capacity() && n < max_size())
			{
				pointer	temp = _alloc.allocate(n);
				for (iterator p = temp, q = _v; p < temp + _capacity; ++p, ++q)
				{
					_alloc.construct(&*p, *q);
					_alloc.destroy(&*q);
				}
				_alloc.deallocate(_v, _capacity);
				_capacity = n;
				_v = temp;
			}
		}

		size_type	capacity() { return _capacity; }

		void	clear() {
			for (iterator p = _v; p < _v + size(); ++p)
				_alloc.destroy(&*p);
			_size = 0;
		}

		// https://stackoverflow.com/questions/40767476/how-does-rhs-work

		iterator	insert(iterator pos, const value_type& value)
		{
			if (pos >= begin() && pos <= end())
			{
				iterator	p, q;

				if (size() == capacity())
				{
					pointer temp = _alloc.allocate(capacity() * 2);
					_capacity *= 2;
					++_size;
					for (p = temp, q = _v; q < pos - 1; ++p, ++q)
					{
						_alloc.construct(&*p, *q);
						_alloc.destroy(&*q);
					}
					_alloc.construct(&*(p++), value);
					difference_type dist = pos - begin();
					for (iterator p2 = p, q2 = q; q2 < begin() + size() + 1 - dist; ++p2, ++q2)
					{
						_alloc.construct(&*p2, *q2);
						_alloc.destroy(&*q2);
					}
					_alloc.deallocate(_v, _capacity);
					_v = temp;
					return (p);
				}
				else
				{
					difference_type dist = pos - 1 - begin();
					iterator p = _v + dist;

					for (iterator q = p + size() - dist; q > p; --q)
						_alloc.construct(&*q, *(q - 1));	
					_alloc.construct(&*p, value);
					++_size;
					return (p);
				}
			}
			else
				throw (ArrayException("out_of_range"));
		}

		void	insert(iterator pos, size_type count, const value_type& value)
		{
			if (pos >= begin() && pos <= end())
			{
				iterator	p, p2, q, q2, r;

				if (size() + count >= capacity())
				{
					size_type	diff = size() + count - capacity();
					pointer temp = _alloc.allocate(capacity() + diff);
					_capacity += diff;
					for (p = temp, q = _v; q < pos - 1; ++p, ++q)
					{
						_alloc.construct(&*p, *q);
						_alloc.destroy(&*q);
					}
					for (p2 = p; count > 0; ++p2, --count)
						_alloc.construct(&*p2, value);
					for (r = p2, q2 = q; q2 < _v + size(); ++r, ++q2)
					{
						_alloc.construct(&*r, *q2);
						_alloc.destroy(&*q2);
					}
					_alloc.deallocate(_v, _capacity);
					_v = temp;
					_size = capacity();
				}
				else
				{
					_size += count;
					difference_type dist = pos - 1 - begin();
					iterator p = _v + dist;

					for (iterator q = p + size(); q > p; --q)
						_alloc.construct(&*q, *(q - count));
					for (iterator r = p; count > 0; ++r, --count)
						_alloc.construct(&*r, value);
				}
			}
			else
				throw (ArrayException("out_of_range"));
		}

		template <class InputIt>
		iterator	insert(iterator pos, InputIt first, InputIt last)
		{
			if (pos >= begin() && pos <= end())
			{
				iterator		p, p2, q, q2, r;
				difference_type	interval = last - first;

				if (size() + interval >= capacity())
				{
					size_type	diff = size() + interval - capacity();
					pointer temp = _alloc.allocate(capacity() + diff);
					_capacity += diff;
					for (p = temp, q = _v; q < pos - 1; ++p, ++q)
					{
						_alloc.construct(&*p, *q);
						_alloc.destroy(&*q);
					}
					for (p2 = p; first < last; ++p2, ++first)
						_alloc.construct(&*p2, *first);
					for (r = p2, q2 = q; q2 < _v + size(); ++r, ++q2)
					{
						_alloc.construct(&*r, *q2);
						_alloc.destroy(&*q2);
					}
					_alloc.deallocate(_v, _capacity);
					_v = temp;
					_size = capacity();
					return (p);
				}
				else
				{
					_size += interval;
					difference_type dist = pos - 1 - begin();
					iterator p = _v + dist;

					for (iterator q = p + size(); q > p; --q)
						_alloc.construct(&*q, *(q - interval));
					for (iterator r = p; first < last; ++r, ++first)
						_alloc.construct(&*r, *first);
					return (p);
				}
			}
			else
				throw (ArrayException("out_of_range"));
		}

		iterator	erase(iterator pos)
		{
			if (pos >= begin() && pos < end())
			{
				difference_type dist = pos - begin();
				iterator p = _v + dist;

				for (iterator q = p; q < p + size(); ++q)
					_alloc.construct(&*q, *(q + 1));
				_alloc.destroy(&*(end()));
				--_size;
				return (p);
			}
			else
				throw (ArrayException("out_of_range"));
		};

		iterator	erase(iterator first, iterator last)
		{
			if ((first >= begin() && first < end()) && (last >= begin() && last < end())
				&& first <= last)
			{
				difference_type	dist = last - first;
				iterator p = first;
				iterator	q, r;

				for (q = p, r = p + dist; r < end(); ++q, ++r)
					_alloc.construct(&*q, *r);
				size_type	size_diff = capacity() - size();
				for (iterator r2 = r; r2 < r2 + size_diff; ++r2)
					_alloc.destroy(&*r2);
				_size -= dist;
				return (p);
			}
			else
				throw (ArrayException("out_of_range"));
		}

		void	push_back(const value_type& value)
		{
			if (_size == capacity()) {
				pointer	temp = _alloc.allocate(_capacity * 2);
				for (iterator p = temp, q = _v; p < temp + size(); ++p, ++q)
				{
					_alloc.construct(&*p, *q);
					_alloc.destroy(&*q);
				}
				_alloc.deallocate(_v, _capacity);
				_capacity *= 2;
				_v = temp;
			}
			_alloc.construct(&*(_v + size()), value);
			++_size;
		}

		void	pop_back()
		{
			if (size())
			{
				pointer	last = _v[size() - 1];

				_alloc.destroy(&*last);
				--_size;
				if (_size < _capacity / 2)
				{
					pointer	temp = _alloc.allocate(_capacity / 2);
					for (iterator p = temp, q = _v; p < temp + size(); ++p, ++q)
					{
						_alloc.construct(&*p, *q);
						_alloc.destroy(&*q);
					}
					_alloc.deallocate(_v, _capacity);
					_capacity /= 2;
					_v = temp;
				}
			}
		}

		void	resize(size_type count, value_type val = value_type())
		{
			if (count <= max_size())
			{
				if (count > capacity())
				{
					iterator	p, q;

					pointer	temp = _alloc.allocate(count); 
					for(p = temp, q = _v; q < end(); ++p, ++q)
						_alloc.construct(&*p, *q);
					for(iterator p2 = p, q2 = q; q2 < _v + count; ++p2, ++q2)
						_alloc.construct(&*p2, val);
					for(iterator r = _v; r < _v + size(); ++r)
						_alloc.destroy(&*r);
					_alloc.deallocate(_v, capacity());
					_v = temp;
					_size = _capacity = count;
				}
				else
				{
					if (count > size())
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
			}
			else
				throw (ArrayException("out_of_range"));
		}

		void	swap(vector& other)
		{
			pointer tmp = other._v;
			other._v = _v;
			_v = tmp;
		}

		bool		operator== (const vector &other) { return this->_v == other._v; }
		bool		operator!= (const vector &other) { return this->_v != other._v; }
		bool		operator< (const vector &other) { return this->_v < other._v; }
		bool		operator<= (const vector &other) { return this->_v <= other._v; }
		bool		operator> (const vector &other) { return this->_v > other._v; }
		bool		operator>= (const vector &other) { return this->_v >= other._v; }
	};
}

#endif