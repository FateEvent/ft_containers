#ifndef VECTOR_H
# define VECTOR_H

# include <iostream>
# include <memory>
# include <algorithm>
# include <cstddef>
# include "ContainerException.hpp"
# include "iterator.hpp"

namespace ft
{
	template <class T, class Allocator = std::allocator<T> > class vector {

	public:
		typedef T										value_type;
		typedef std::size_t								size_type;
		typedef Allocator								allocator_type;
		typedef std::ptrdiff_t							difference_type;
		typedef value_type&								reference;
		typedef const value_type&						const_reference;
		typedef typename Allocator::pointer				pointer;
		typedef typename Allocator::const_pointer		const_pointer;		
		typedef ft::move_iterator<value_type>			iterator;
		typedef ft::const_iter<value_type>				const_iterator;
		typedef ft::reverse_iterator<iterator>			reverse_iterator;
		typedef ft::reverse_iterator<const_iterator>	const_reverse_iterator;

	public:
		vector() : _size(), _capacity(), _v(NULL) {}

		explicit vector( const Allocator& alloc ) : _size(), _capacity(), _alloc(alloc), _v(NULL) {}

		explicit vector( size_type count, const value_type& value = value_type(), const Allocator& alloc = Allocator() ) : _alloc(alloc), _v(NULL)
		{
			_size = _capacity = count;
			_v = _alloc.allocate(_capacity);
			for (iterator p = _v; p < _v + _capacity; ++p)
				_alloc.construct(&*p, value);
		}

		template <class InputIt>
		vector( InputIt first, InputIt last, const Allocator& alloc = Allocator() ) : _alloc(alloc), _v(NULL)
		{
			ptrdiff_t	dist = last - first;
			_size = _capacity = dist;
			_v = _alloc.allocate(capacity());
			std::copy(first, last, _v);
		}

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
			_alloc.deallocate(_v, capacity());
		}

		vector&	operator= ( const vector& rhs )
		{
			if (this == &rhs)
				return (*this);
			for (iterator p = _v; p < _v + size(); ++p)
				_alloc.destroy(&*p);
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

		void	assign( size_type count, const T& value ) {
			for (iterator p = _v; p < _v + size(); ++p)
				_alloc.destroy(&*p);
			_alloc.deallocate(_v, capacity());
			_size = _capacity = count;
			pointer	temp = _alloc.allocate(count);
			for (iterator p = temp; p < temp + count; ++p)
				_alloc.construct(&*p, value);
			_v = temp;
		}

		template <class InputIt >
		void	assign( InputIt first, InputIt last )
		{
			pointer	temp;

			for (iterator p = _v; p < _v + size(); ++p)
				_alloc.destroy(&*p);
			_alloc.deallocate(_v, capacity());
			ptrdiff_t	dist = last - first;
			if (dist > 0)
			{
				_size = _capacity = dist;
				try
				{
					temp = _alloc.allocate(dist);
				}
				catch (std::exception& e)
				{
					std::cout << "Standard exception: " << e.what() << std::endl;
				}
				for (iterator p = temp; p < temp + dist && first < last; ++p, ++first)
					_alloc.construct(&*p, *first);
				_v = temp;
			}
			else
				throw(ContainerException("out_of_range"));
			
		}

		allocator_type	get_allocator() const { return _alloc; }

		reference	at( size_type pos ) {
			if (pos >= size())
				throw (ContainerException("out_of_range"));
			else
				return (_v[pos]);
		};
		
		const_reference	at( size_type pos ) const {
			if (pos >= size())
				throw (ContainerException("out_of_range"));
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

		iterator				begin()	{ return iterator(&_v[0]); }
		const_iterator			begin() const { return const_iterator(&(static_cast<pointer>(_v)[0])); }
		iterator				end() { return iterator(&_v[_size]); }
		const_iterator			end() const { return const_iterator(&(static_cast<pointer>(_v)[_size])); }

		reverse_iterator		rbegin() { return reverse_iterator(&_v[_size - 1]); }
		const_reverse_iterator	rbegin() const { return const_reverse_iterator(&(static_cast<pointer>(_v)[_size - 1])); }
		reverse_iterator		rend() { return reverse_iterator(&_v[-1]); }
		const_reverse_iterator	rend() const	{ return const_reverse_iterator(&(static_cast<pointer>(_v)[-1])); }
		
		bool empty() const { return !size(); }

		size_type	size() const { return _size; }

		size_type	max_size() const { return std::min<size_type>(_alloc.max_size(),
								std::numeric_limits<difference_type>::max()); }

		void	reserve(size_type n)
		{
			if (n > capacity() && n < capacity() * 2 && n <= max_size())
			{
				pointer	temp = _alloc.allocate(capacity() * 2);
				iterator p = temp;

				std::copy(begin(), end(), p);
				_alloc.deallocate(_v, capacity());
				_capacity *= 2;
				_v = temp;
			}
			else if (n > capacity() * 2 && n <= max_size())
			{
				pointer	temp = _alloc.allocate(n);
				for (iterator p = temp, q = _v; p < temp + capacity(); ++p, ++q)
				{
					_alloc.construct(&*p, *q);
					_alloc.destroy(&*q);
				}
				_alloc.deallocate(_v, capacity());
				_capacity = n;
				_v = temp;
			}
			else if (n > max_size())
				throw (ContainerException("out_of_range"));
		}

		size_type	capacity() const { return _capacity; }

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
					_alloc.deallocate(_v, capacity());
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
				throw (ContainerException("out_of_range"));
		}

		void	insert(iterator pos, size_type count, const value_type& value)
		{
			if (pos >= begin() && pos <= end())
			{
				iterator	p, q, r;

				if (size() + count >= capacity())
				{
					size_type diff = size() + count - capacity();
					pointer temp = _alloc.allocate(capacity() + diff);
					_capacity += diff;
					for (p = temp, q = _v; q < pos - 1; ++p, ++q)
					{
						_alloc.construct(&*p, *q);
						_alloc.destroy(&*q);
					}
					for (; count > 0; ++p, --count)
						_alloc.construct(&*p, value);
					for (; q < _v + size(); ++p, ++q)
					{
						_alloc.construct(&*p, *q);
						_alloc.destroy(&*q);
					}
					_alloc.deallocate(_v, capacity());
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
				throw (ContainerException("out_of_range"));
		}

		template <class InputIt>
		iterator	insert(iterator pos, InputIt first, InputIt last)
		{
			if (pos >= begin() && pos <= end())
			{
				difference_type	dist = pos - 1 - begin();
				difference_type	interval = last - first;

				if (size() + interval >= capacity())
				{
					size_type	diff = size() + interval - capacity();
					pointer temp = _alloc.allocate(capacity() + diff);
					_capacity += diff;
					std::copy(begin(), pos - 1, temp);
					std::copy(first, last, temp + dist);
					std::copy(pos - 1, end(), temp + dist + interval);
					_alloc.deallocate(_v, capacity());
					_v = temp;
					_size = capacity();
					iterator p = _v + dist;
					return (p);
				}
				else
				{
					iterator p = _v + dist;
					_size += interval;

					std::copy(pos - 1, end(), end() - 1);
					std::copy(first, last, pos - 1);
					return (p);
				}
			}
			else
				throw (ContainerException("out_of_range"));
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
				throw (ContainerException("out_of_range"));
		};

		iterator	erase(iterator first, iterator last)
		{
			if ((first >= begin() && first < end()) && (last >= begin() && last < end())
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
				throw (ContainerException("out_of_range"));
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
				_alloc.deallocate(_v, capacity());
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
				if (size() == capacity() / 2)
				{
					pointer	temp = _alloc.allocate(capacity() / 2);
					for (iterator p = temp, q = _v; p < temp + size(); ++p, ++q)
					{
						_alloc.construct(&*p, *q);
						_alloc.destroy(&*q);
					}
					_alloc.deallocate(_v, capacity());
					_capacity /= 2;
					_v = temp;
				}
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
				throw (ContainerException("out_of_range"));
		}

		void	swap(vector& other)
		{
			pointer tmp = other._v;
			other._v = _v;
			_v = tmp;
			size_type temp = other._size;
			other._size = _size;
			_size = temp;
			temp = other._capacity;
			other._capacity = _capacity;
			_capacity = other._capacity;
		}

	private:
		size_type		_size;
		size_type		_capacity;
		allocator_type	_alloc;
		pointer			_v;
	};

	template<class T, class Alloc>
	bool	operator== (const ft::vector<T, Alloc> &lhs, const ft::vector<T, Alloc> &rhs) { return lhs->_v == rhs->_v; }
	template<class T, class Alloc>
	bool	operator!= (const ft::vector<T, Alloc> &lhs, const ft::vector<T, Alloc> &rhs) { return lhs->_v != rhs->_v; }
	template<class T, class Alloc>
	bool	operator< (const ft::vector<T, Alloc> &lhs, const ft::vector<T, Alloc> &rhs) { return lhs->_v < rhs->_v; }
	template<class T, class Alloc>
	bool	operator<= (const ft::vector<T, Alloc> &lhs, const ft::vector<T, Alloc> &rhs) { return lhs->_v <= rhs->_v; }
	template<class T, class Alloc>
	bool	operator> (const ft::vector<T, Alloc> &lhs, const ft::vector<T, Alloc> &rhs) { return lhs->_v > rhs->_v; }
	template<class T, class Alloc>
	bool	operator>= (const ft::vector<T, Alloc> &lhs, const ft::vector<T, Alloc> &rhs) { return lhs->_v >= rhs->_v; }
}

#endif