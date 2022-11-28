#ifndef VECTOR_H
# define VECTOR_H

# include <iostream>
# include <memory>
# include "iterator.hpp"
#include <iterator>
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
		typedef ft::iterator<value_type>				iterator;
		typedef ft::const_iterator<value_type>			const_iterator;
		typedef ft::reverse_iterator<value_type>		reverse_iterator;
		typedef ft::const_reverse_iterator<value_type>	const_reverse_iterator;

	private:
		T*			arr;
		int 		capacity;
		int 		current;
		Allocator	allocator;
		iterator	v;

	public:
		vector() : v(nullptr)
		{
			v = allocator.allocate(1);
		}

		explicit vector( const Allocator& alloc ) : allocator(alloc), v(nullptr)
		{
			v = allocator.allocate(1);
		};

		explicit vector( size_type count, const T& value = T(), const Allocator& alloc = Allocator() ) : allocator(alloc), v(nullptr)
		{
			v = allocator.allocate(count);
			for (iterator p = v; p < v + count; ++p)
				allocator.construct(p, value);
		}

		template <class InputIt>
		vector( InputIt first, InputIt last, const Allocator& alloc = Allocator() ) : allocator(alloc), v(nullptr)
		{
			ptrdiff_t	dist = last - first;
			v = allocator.allocate(dist);
			for (iterator p = v; p < v + dist && first < last; p++)
			{
				alloc.construct(p, *first);
				first++;
			}
		};

		vector( const vector& other );

		~vector()
		{
			delete[] arr;
		}

		void push(T data)
		{
			if (current == capacity) {
				T* temp = new T[capacity + 1];
				for (int i = 0; i < capacity; i++) {
					temp[i] = arr[i];
				}
				delete[] arr;
				capacity += 1;
				arr = temp;
			}
			arr[current] = data;
			current++;
		}

		void push(T data, int index)
		{
			if (index == capacity)
				push(data);
			else
				arr[index] = data;
		}

		T get(int index)
		{
			if (index < current)
				return arr[index];
			throw (ArrayException("MOVC::InvalidIndexException"));
		}

		void pop()
		{
			T* temp = new T[capacity - 1];
			for (int i = 0; i < capacity - 1; i++) {
				temp[i] = arr[i];
			}
			delete[] arr;
			capacity -= 1;
			arr = temp;
			current--;
		}

		int size() { return current; }

		int getcapacity() { return capacity; }

		void print()
		{
			for (int i = 0; i < current; i++) {
				std::cout << arr[i] << " ";
			}
			std::cout << std::endl;
		}

		iterator		begin()	{ return iterator(&arr[0]); }
		const_iterator	begin() const	{ return const_iterator(&arr[0]); }
		iterator		end()	{ return iterator(&arr[size()]); }
		const_iterator	end() const	{ return const_iterator(&arr[size()]); }

		reverse_iterator		rbegin()	{ return reverse_iterator(&arr[size() - 1]); }
		const_reverse_iterator	rbegin() const	{ return const_reverse_iterator(&arr[size() - 1]); }
		reverse_iterator		rend()	{ return reverse_iterator(&arr[-1]); }
		const_reverse_iterator	rend() const	{ return const_reverse_iterator(&arr[-1]); }

		reference	operator[]( size_type pos ) { return &arr[pos]; };
		const_reference	operator[]( size_type pos ) const { return &arr[pos]; }

		reference	at( size_type pos ) {
			if (pos >= size())
				throw (ArrayException("out_of_range"));
			return (&arr[pos]);
		};
		
		const_reference	at( size_type pos ) const {
			if (pos >= size())
				throw (ArrayException("out_of_range"));
			return (&arr[pos]);
		};

	};
}

#endif