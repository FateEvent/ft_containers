#ifndef ITERATOR_H
# define ITERATOR_H

# include <iostream>
# include <memory>
# include "ArrayException.hpp"

namespace ft
{
	template <class T, class Allocator = std::allocator<T> >
	class vector
	{
	private:
		T* arr;
		int capacity;
		int current;

	public:
		typedef T									value_type;
		typedef std::size_t							size_type;
		typedef Allocator							allocator_type;
		typedef std::ptrdiff_t						difference_type;
		typedef value_type&							reference;
		typedef const value_type&					const_reference;
		typedef typename Allocator::pointer			pointer;
		typedef typename Allocator::const_pointer	const_pointer;
//		iterator
//		const_iterator
//		reverse_iterator
//		const_reverse_iterator


		vector()
		{
			arr = new T[1];
			capacity = 1;
			current = 0;
		}
		~vector()
		{
			delete [] arr;
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

		typedef struct Iterator
		{
			Iterator(int *ptr) : _ptr(ptr) {}

			int	&operator * () const { return *_ptr; }
			int	*operator -> () { return _ptr; }

			Iterator&	operator ++() { _ptr++; return *this; }
			Iterator	operator ++(int) { Iterator tmp = *this; ++(*this); return tmp; }
			Iterator&	operator --() { _ptr--; return *this; }
			Iterator	operator --(int) { Iterator tmp = *this; --(*this); return tmp; }

			bool operator == (const Iterator& it) { return this->_ptr == it._ptr; };
			bool operator != (const Iterator& it) { return this->_ptr != it._ptr; };

		private:
			int	*_ptr;

		}				iterator;

		Iterator	begin()	{ return Iterator(&arr[0]); }
		Iterator	end()	{ return Iterator(&arr[size()]); }
	};
}

#endif