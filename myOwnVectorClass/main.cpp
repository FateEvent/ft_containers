// Self implementation of
// the Vector Class in C++

#include <iostream>
#include "ArrayException.hpp"

template <typename T> class MyOwnVectorClass {

private:
	// arr is the integer pointer
	// which stores the address of our vector
	T* arr;

	// capacity is the total storage
	// capacity of the vector
	int capacity;

	// current is the number of elements
	// currently present in the vector
	int current;

public:
	// Default constructor to initialise
	// an initial capacity of 1 element and
	// allocating storage using dynamic allocation
	MyOwnVectorClass()
	{
		arr = new T[1];
		capacity = 1;
		current = 0;
	}
	//destructor to deallocate storage allocated by dynamic allocation
	//to prevent memory leak
	~ MyOwnVectorClass()
	{
		delete [] arr;
	}

	// Function to add an element at the last
	void push(T data)
	{

		// if the number of elements is equal to the
		// capacity, that means we don't have space to
		// accommodate more elements. We need to double the
		// capacity
		if (current == capacity) {
			T* temp = new T[capacity + 1];

			// copying old array elements to new array
			for (int i = 0; i < capacity; i++) {
				temp[i] = arr[i];
			}

			// deleting previous array
			delete[] arr;
			capacity += 1;
			arr = temp;
		}

		// Inserting data
		arr[current] = data;
		current++;
	}

	// function to add element at any index
	void push(T data, int index)
	{

		// if index is equal to capacity then this
		// function is same as push defined above
		if (index == capacity)
			push(data);
		else
			arr[index] = data;
	}

	// function to extract element at any index
	T get(int index)
	{
		if (index < current)
			return arr[index];
		throw (ArrayException("MOVC::InvalidIndexException"));
	}

	// function to delete last element
	void pop()
	{
		T* temp = new T[capacity - 1];

		// copying old array elements to new array
		for (int i = 0; i < capacity - 1; i++) {
			temp[i] = arr[i];
		}

		// deleting previous array
		delete[] arr;
		capacity -= 1;
		arr = temp;
		current--;
	}

	// function to get size of the vector
	int size() { return current; }

	// function to get capacity of the vector
	int getcapacity() { return capacity; }

	// function to print array elements
	void print()
	{
		for (int i = 0; i < current; i++) {
			std::cout << arr[i] << " ";
		}
		std::cout << std::endl;
	}

	typedef struct Iterator
	{
		Iterator(int	*ptr) : _ptr(ptr) {}

		int	&operator * () const { return *_ptr; }
		int	*operator -> () { return _ptr; }

		// Prefix increment
		Iterator& operator ++() { _ptr++; return *this; }

		// Postfix increment
		Iterator operator ++(int) { Iterator tmp = *this; ++(*this); return tmp; }

		Iterator&	operator --() { _ptr--; return *this; }
		Iterator	operator --(int) { Iterator tmp = *this; --(*this); return tmp; }

		bool operator == (const Iterator& it) { return this->_ptr == it._ptr; };
		bool operator != (const Iterator& it) { return this->_ptr != it._ptr; };

	private:
		int	*_ptr;

	}								iterator;

	Iterator	begin()	{ return Iterator(&arr[0]); }
	Iterator	end()		{ return Iterator(&arr[size()]); }

};

// Driver code
int main()
{
	MyOwnVectorClass<int> v;
	MyOwnVectorClass<char> v1;
	v.push(10);
	v.push(20);
	v.push(30);
	v.push(40);
	v.push(50);
	v1.push(71);
	v1.push(72);
	v1.push(73);
	v1.push(74);


	std::cout << "The vector contains the following elements:" << std::endl;
	MyOwnVectorClass<int>::iterator it = v.begin();
	MyOwnVectorClass<int>::iterator ite = v.end();
	
	++it;
	--it;
	while (it != ite)
	{
		std::cout << *it << std::endl;
		++it;
	}

	std::cout << "Vector size : " << v.size() << std::endl;
	std::cout << "Vector capacity : " << v.getcapacity() << std::endl;

	std::cout << "Vector elements : ";
	v.print();

	v.push(100, 1);

	std::cout << "\nAfter updating 1st index" << std::endl;

	std::cout << "Vector elements of type int : " << std::endl;
	v.print();
	// This was possible because we used templates
	std::cout << "Vector elements of type char : " << std::endl;
	v1.print();
	std::cout << "Element at 1st index of type int: " << v.get(1)
		<< std::endl;
	std::cout << "Element at 1st index of type char: "
		<< v1.get(1) << std::endl;

	v.pop();
	v1.pop();

	std::cout << "\nAfter deleting last element" << std::endl;

	std::cout << "Vector size of type int: " << v.size() << std::endl;
	std::cout << "Vector size of type char: " << v1.size()
		<< std::endl;
	std::cout << "Vector capacity of type int : "
		<< v.getcapacity() << std::endl;
	std::cout << "Vector capacity of type char : "
		<< v1.getcapacity() << std::endl;

	std::cout << "Vector elements of type int: ";
	v.print();
	std::cout << "Vector elements of type char: ";
	v1.print();

	try {
		std::cout << v1.get(10) << std::endl;
	}
	catch(std::exception &e) {
		std::cerr << e.what() << std::endl;
	}

	return 0;
}
