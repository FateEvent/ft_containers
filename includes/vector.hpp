#ifndef VECTOR_H
# define VECTOR_H

# include <iostream>
# include <memory>
# include "iterator.hpp"
# include "ArrayException.hpp"

namespace ft
{
	template <class T, class Allocator = std::allocator<T> > class vector {
	private:
		T*			arr;
		int 		capacity;
		int 		current;
		Allocator	alloc;
		iterator	v;

	public:
		typedef T									value_type;
		typedef std::size_t							size_type;
		typedef Allocator							allocator_type;
		typedef std::ptrdiff_t						difference_type;
		typedef value_type&							reference;
		typedef const value_type&					const_reference;
		typedef typename Allocator::pointer			pointer;
		typedef typename Allocator::const_pointer	const_pointer;
		typedef ft::iterator						iterator;
		typedef ft::const_iterator					const_iterator;
		typedef ft::reverse_iterator				reverse_iterator;
		typedef ft::const_reverse_iterator			const_reverse_iterator;

		vector ( size_type count, const T& value = T(), const Allocator& alloc = Allocator() ) : alloc(alloc)
		{
			v = alloc.allocate(count);
			for(iterator p=v;p<v+count;++p)alloc.construct(p,value);
		}
		vector()
		{
			arr = new T[1];
			capacity = 1;
			current = 0;
		}
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

		iterator	begin()	{ return Iterator(&arr[0]); }
		iterator	end()	{ return Iterator(&arr[size()]); }
	};
}

#endif