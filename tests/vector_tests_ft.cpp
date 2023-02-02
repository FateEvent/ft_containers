#include <iostream>
#include <string>
#include <stdlib.h>
#include <vector>
#include <stack.hpp>
#include <vector.hpp>

#define MAX_RAM 4294967296
#define BUFFER_SIZE 4096
struct Buffer
{
	int idx;
	char buff[BUFFER_SIZE];
};

#define COUNT (MAX_RAM / (int)sizeof(Buffer))

template<typename T>
class MutantStack : public ft::stack<T>
{
public:
	MutantStack() {}
	MutantStack(const MutantStack<T>& src) { *this = src; }
	MutantStack<T>& operator=(const MutantStack<T>& rhs) 
	{
		this->c = rhs.c;
		return *this;
	}
	~MutantStack() {}

	typedef typename ft::stack<T>::container_type::iterator iterator;

	iterator begin() { return this->c.begin(); }
	iterator end() { return this->c.end(); }
};

template<class T>
void	vector_print(ft::vector<T> vec)
{
	std::cout << "size and capacity: " << vec.size() << ", " << vec.capacity() << std::endl;
	std::cout << "The maximum storage capacity of the container is " << vec.max_size() << std::endl;
	std::cout << "What the vector contains:" << std::endl;
	for (typename ft::vector<T>::iterator it = vec.begin(); it != vec.end(); it++)
		std::cout << *it << std::endl;
}

int main() {
	srand(time(0));

	ft::vector<std::string> vector_str;
	ft::vector<int> vector_int;
	ft::stack<int> stack_int;
	ft::vector<Buffer> vector_buffer;
	ft::stack<Buffer, std::vector<Buffer> > stack_deq_buffer;

	for (int i = 0; i < COUNT; i++)
		vector_buffer.push_back(Buffer());

	for (int i = 0; i < COUNT; i++)
	{
		const int idx = rand() % COUNT;
		vector_buffer[idx].idx = 5;
	}
	ft::vector<Buffer>().swap(vector_buffer);

	try
	{
		for (int i = 0; i < COUNT; i++)
		{
			const int idx = rand() % COUNT;
			vector_buffer.at(idx);
			std::cout << vector_buffer.size() <<std::endl;
		}
	}
	catch(const std::exception& e)
	{
		//NORMAL ! :P
	}

	std::cout << "// --> My tests <-- //" << std::endl << std::endl;

	std::cout << "// --> Vector <-- //" << std::endl << std::endl;

	MutantStack<char> iterable_stack;
	for (char letter = 'a'; letter <= 'z'; letter++)
		iterable_stack.push(letter);
	for (MutantStack<char>::iterator it = iterable_stack.begin(); it != iterable_stack.end(); it++)
		std::cout << *it;
	std::cout << std::endl;

	MutantStack<char>::iterator itLetter = iterable_stack.begin();
	MutantStack<char>::iterator itLetter2 = iterable_stack.end();

	ft::vector<char>	newVector(itLetter, itLetter2);

	vector_print(newVector);

	newVector.push_back('j');
	for (ft::vector<char>::iterator it = newVector.end() - 2; it != newVector.end(); it++)
		std::cout << "last letters: " << *it << std::endl;

	ft::vector<char>	vecCopy(newVector);
	vector_print(vecCopy);

	vecCopy.assign(4, 'a');
	vector_print(vecCopy);

	newVector.reserve(45);
	newVector.assign(vecCopy.begin(), vecCopy.end());
	newVector.insert((newVector.begin() + 2), 'z');
	newVector.push_back('c');
	vector_print(newVector);

	vecCopy.push_back('c');
	vecCopy.push_back('c');
	vecCopy.push_back('c');
	vecCopy.push_back('c');
	vecCopy.insert(vecCopy.begin() + 2, itLetter, itLetter2);
	vector_print(vecCopy);

	vecCopy.insert(vecCopy.begin() + 2, 5, 'Z');
	vector_print(vecCopy);

	vecCopy.insert(vecCopy.begin() + 2, 'x');
	std::cout << "return: " << *(vecCopy.erase(vecCopy.begin() + 1, vecCopy.begin() + 7)) << std::endl;
	std::cout << "return: " << *(vecCopy.erase(vecCopy.begin() + 1)) << std::endl;
	vecCopy.resize(35);
	vector_print(vecCopy);
	
	vecCopy.swap(newVector);
	vector_print(vecCopy);

	ft::vector<char>	newDeck(itLetter, itLetter2);
	vector_print(newDeck);

	newDeck.resize(53);
	vector_print(newDeck);

	vecCopy[4] = 'c';
	vecCopy[3] = 'l';
	vector_print(vecCopy);
}
