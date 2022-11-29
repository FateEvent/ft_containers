#ifndef VECTOR_H
# define VECTOR_H

# include <iostream>
# include <memory>
# include "iterator.hpp"
#include <vector>
# include "ArrayException.hpp"

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
		typedef ft::const_iterator<value_type>			const_iterator;
		typedef ft::reverse_iterator<value_type>		reverse_iterator;
		typedef ft::const_reverse_iterator<value_type>	const_reverse_iterator;

	private:
		size_type	_size;
		size_type	_max_size;
		size_type	_capacity;
		Allocator	_alloc;
		pointer		_v;

	public:
		vector() : _size(), _capacity(), _v(nullptr) {}

		explicit vector( const Allocator& alloc ) : _size(), _capacity(), _alloc(alloc), _v(nullptr) {}

		explicit vector( size_type count, const T& value = T(), const Allocator& alloc = Allocator() ) : _alloc(alloc), _v(nullptr)
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
			for (iterator p = _v; p < _v + dist && first < last; ++p, first++)
			{
				_alloc.construct(&*p, *first);
			}
		};

		vector( const vector& other )
		{

		};

		operator= ( const vector& other )
		{
			
		}

		~vector()
		{
			_alloc.deallocate(_v, _capacity);
		}

		void push(T data)
		{
			if (_size == _capacity) {
				T* temp = new T[_capacity + 1];
				for (int i = 0; i < _capacity; i++) {
					temp[i] = _v[i];
				}
				delete[] _v;
				_capacity += 1;
				_v = temp;
			}
			_v[_size] = data;
			_size++;
		}

		void push(T data, size_type index)
		{
			if (index == _capacity)
				push(data);
			else
				_v[index] = data;
		}

		T get(size_type index)
		{
			if (index < _size)
				return _v[index];
			throw (ArrayException("MOVC::InvalidIndexException"));
		}

		void pop()
		{
			T* temp = new T[_capacity - 1];
			for (int i = 0; i < _capacity - 1; i++) {
				temp[i] = _v[i];
			}
			delete[] _v;
			_capacity -= 1;
			_v = temp;
			_size--;
		}

		int size() { return _size; }

		int get_capacity() { return _capacity; }

		void print()
		{
			for (int i = 0; i < _size; i++) {
				std::cout << _v[i] << " ";
			}
			std::cout << std::endl;
		}

		iterator				begin()	{ return iterator(&_v[0]); }
		const_iterator			begin() const	{ return const_iterator(&_v[0]); }
		iterator				end()	{ return iterator(&_v[_size]); }
		const_iterator			end() const	{ return const_iterator(&_v[_size]); }

		reverse_iterator		rbegin()	{ return reverse_iterator(&_v[_size - 1]); }
		const_reverse_iterator	rbegin() const	{ return const_reverse_iterator(&_v[_size - 1]); }
		reverse_iterator		rend()	{ return reverse_iterator(&_v[-1]); }
		const_reverse_iterator	rend() const	{ return const_reverse_iterator(&_v[-1]); }

		reference				operator[]( size_type pos ) { return &_v[pos]; };
		const_reference			operator[]( size_type pos ) const { return &_v[pos]; }

		reference	at( size_type pos ) {
			if (pos >= _size)
				throw (ArrayException("out_of_range"));
			return (&_v[pos]);
		};
		
		const_reference	at( size_type pos ) const {
			if (pos >= _size)
				throw (ArrayException("out_of_range"));
			return (&_v[pos]);
		};

		value_type	&operator++ () { _v++; return *_v; }
		value_type	operator++ (int) { value_type tmp = *_v; ++_v; return tmp; }
		value_type	&operator-- () { _v--; return *_v; }
		value_type	operator-- (int) { value_type tmp = *_v; --_v; return tmp; }		
		value_type	&operator+= (std::size_t dist) { _v += dist; return *_v; }
		value_type	&operator-= (std::size_t dist) { _v -= dist; return *_v; }
		value_type	operator+ (std::size_t dist) const { return (_v + dist); }
		value_type	operator- (std::size_t dist) const { return (_v - dist); }

		bool		operator== (const vector &other) { return this->_v == other._v; }
		bool		operator!= (const vector &other) { return this->_v != other._v; }
		bool		operator< (const vector &other) { return this->_v < other._v; }
		bool		operator> (const vector &other) { return this->_v > other._v; }
		bool		operator<= (const vector &other) { return this->_v <= other._v; }
		bool		operator>= (const vector &other) { return this->_v >= other._v; }
	};
}

#endif